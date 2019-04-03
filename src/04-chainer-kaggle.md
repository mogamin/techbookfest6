# kaggleにsubmitしよう！

@<icon>{cheiko} 「せんせい、この章では何をやるの？」

//blankline

@<icon>{yousei} 「この章ではChainerを使ってkaggleに参加してみるよ！」

//blankline

@<icon>{cheiko} 「kaggle？？？」

//blankline

@<icon>{yousei} 「kaggleはデータサイエンスの問題でスコアを競うオンラインコンペティションだよ〜〜」

//blankline

ちぇい子は知っていたような顔をした。

//blankline

@<icon>{yousei} 「コンペティションにsubmitするには、Chainerで使えるようにデータを読み込んで学習を行ったあと、課題のデータに対して予測・推論をする必要があるんだ。」

//blankline

※この章ではGoogleColabratory@<fn>{fn40}上で実行することを想定しています。
//footnote[fn40][https://colab.research.google.com/]

## kaggleコマンドの準備
準備として、kaggleのデータセットを利用して何かしらの結果を提出してみましょう。@<fn>{fn41}
//footnote[fn41][手順はhttps://qiita.com/fam_taro/items/eb9bae0b82248f9abd28を参考にしています]

### kaggle コマンド(api) のインストール

```
!pip install kaggle
```

### Kaggle API Key の入手

* https://www.kaggle.com/UserName/account で自分のアカウントページを開いてください
* 上記ページ内の Create New API Token をクリックして kaggle.json をダウンロードしてください

### Google ドライブに kaggle.json をアップロード

* https://drive.google.com/drive/my-drive を開いてください
* kaggle.json をアップロードしてください

### Google ドライブから Colaboratory 上に kaggle.json をダウンロード

```
from googleapiclient.discovery import build
import io, os
from googleapiclient.http import MediaIoBaseDownload
from google.colab import auth

auth.authenticate_user()

drive_service = build('drive', 'v3')
results = drive_service.files().list(
        q="name = 'kaggle.json'", fields="files(id)").execute()
kaggle_api_key = results.get('files', [])

filename = "/root/.kaggle/kaggle.json"
os.makedirs(os.path.dirname(filename), exist_ok=True)

request = drive_service.files().get_media(
    fileId=kaggle_api_key[0]['id']
)
fh = io.FileIO(filename, 'wb')
downloader = MediaIoBaseDownload(fh, request)
done = False
while done is False:
    status, done = downloader.next_chunk()
    print("Download %d%%." % int(status.progress() * 100))
os.chmod(filename, 600)
```

### 各種コマンド操作

* digit-recognizerのデータセットのダウンロード(予めコンペへの参加 (コンペルールへの同意) は済ませる必要がります)

```
!kaggle competitions download -c digit-recognizer
```

* コンペに提出

```
!kaggle competitions submit -c digit-recognizer \
    -f /content/sample_submission.csv -m "Test"
```

## 自分でDatasetを作ってみよう

教師あり学習の場合、**データセットは「入力データ」と「それと対になるラベルデータ」を返すオブジェクトである必要があります。**
ChainerにはMNISTやCIFAR10/100のようなよく用いられるデータセットに対して、データをダウンロードしてくるところからそのような機能をもったオブジェクトを作るところまで、自動的にやってくれる便利なメソッドがあります。

しかし、好きな問題を解くには自分でデータセットを作る必要があります。この節ではどのようにDatasetを作るか説明します。

まずは、pandasを使ってcsvを読み込んでみましょう。訓練用データとテスト用データはそれぞれ以下のパスにダウンロードされています。

```
train_path = '/content/train.csv'
test_path = '/content/test.csv'
```

`pandas`を使ってcsvを読み込んでみましょう。`pandas`はpythonで頻繁にしようされるデータ操作・分析ライブラリです。`read_csv`で簡単にcsvファイルを読み込むことができます。

```
df_train_val = pd.read_csv(train_path)
df_test = pd.read_csv(test_path)
```

Chainerで使用するデータセットのデータ構造は単純で、`train_val[i]`などとするとi番目の (data, label) というタプルを返すリストと同様のものになっています（実際ただのPythonリストもChainerのデータセットオブジェクトとして使えます)。

 以下の`create_dataset`は上記で作成したDataFrameからChainerに則したデータセットを作る関数です。関数は以下のことに注意して定義する必要があります。
 
 * dataset[i]などとするとi番目の (data, label) というタプルを返すリストになっていること。
 * dataはfloat32型、labelはint32型であること。
 * 元画素値が0~255のint値だが、それを0~1のfloat値になるようにスケールすること。
 
```
def create_dataset(df, is_test = False):
    dataset = []
    for index, row in df.iterrows():
        data = (
            row.iloc[1:].values.astype('float32') /255.0,
            row.iloc[0].astype('int32')
        )
        dataset.append(data)
    return dataset
```

`create_dataset`を使って、chainer形式のデータセット`train_val`を作りましょう。

```
train_val = create_dataset(df_train_val)
```

ここでは、`30.000`を訓練用データ、残りを検証用データに分割してみましょう。

```
train_size = 30000
train, validation = chainer.datasets.split_dataset_random(
    train_val, train_size, seed=0)
```

ここでは、訓練と検証を行う関数`train_and_validate`を定義します。
詳しい説明は前回のハンズオンの資料を参照ください。

```
from chainer import optimizers, training
from chainer.training import extensions
    
def  train_and_validate(
    model, optimizer, train, validation,
    n_epoch, batchsize, device=0
):
    # 1. deviceがgpuであれば、gpuにモデルのデータを転送する
    if device >= 0:
        model.to_gpu(device)
    # 2. Optimizerを設定する
    optimizer.setup(model)
    # 3. DatasetからIteratorを作成する
    train_iter = chainer.iterators.SerialIterator(train, batchsize)
    validation_iter = chainer.iterators.SerialIterator(
        validation, batchsize, repeat=False, shuffle=False)
    # 4. Updater・Trainerを作成する
    updater = training.StandardUpdater(
        train_iter, optimizer, device=device)
    trainer = chainer.training.Trainer(
        updater, (n_epoch, 'epoch'), out='out')
    # 5. Trainerの機能を拡張する
    trainer.extend(extensions.LogReport())
    trainer.extend(extensions.Evaluator(
        validation_iter, model, device=device), name='val')
    trainer.extend(extensions.PrintReport(
        ['epoch', 'main/loss', 'main/accuracy',
         'val/main/loss', 'val/main/accuracy']))
    trainer.extend(extensions.PlotReport(
        ['main/loss', 'val/main/loss'],x_key='epoch',
        file_name='loss.png'))
    trainer.extend(extensions.PlotReport(
        ['main/accuracy', 'val/main/accuracy'], x_key='epoch',
        file_name='accuracy.png'))
    trainer.extend(extensions.dump_graph('main/loss'))
    # 6. 訓練を開始する
    trainer.run()
```

ここでは、最も単純なニューラルネットワークモデルとして、2層の多層パーセプトロン（MLP2）を作ってみましょう。

```
import chainer.functions as F
import chainer.links as L
    
class MLP(Chain):

    def __init__(self):
        super(MLP2, self).__init__()
        with self.init_scope():
            self.l1=L.Linear(None, 200)
            self.l2=L.Linear(None, 10)

    def forward(self, x):
        h1 = F.tanh(self.l1(x))
        y = self.l2(h1)
        return y
```

```
n_epoch = 10
batchsize = 128
model = MLP()
classifier_model = L.Classifier(model)
optimizer = optimizers.Adam()
train_and_validate(
    classifier_model, optimizer, train, validation,
    n_epoch, batchsize
)
```

kaggleに結果をsubmitするためには、テストデータに対して作成したモデルで推論を行う必要があります。 以下はそのコードになります。関数は以下の引数によって`predictions = make_prediction(model, test)`と呼び出されることを想定しています。

* `test = (df_test.values / 255.0).astype("float32")`で生成されるnumpy配列
*  `model = MyConvNet()`で生成されたあと、既に学習が済んだモデル。注意が必要なのは、`L.Classifier`でwrapされる前なので、softmax関数やcross-entoropy関数を内部に持たない。

```
import cupy as cp

def make_prediction(
    model, test, device = 0, batchsize = 256
):
    if device >= 0:
        model = model.to_gpu()
        xp = cp
    else:
        model = model.to_cpu()
        xp = np
   
    # 指定したbatchsizeごとにデータを分割している
    dataset = [
        xp.array(
            test[
                i * batchsize : (i + 1) * batchsize, :
            ]
        )
        for i in range((len(test) // batchsize + 1))
    ]
    
    predictions = []
    for d in dataset:
        with chainer.configuration.using_config('train', False):
            y = model(d)
            p = y.array.argmax(axis=1).astype("int32")
        predictions.extend(p)

    return predictions
```

```
test = (df_test.values / 255.0).astype("float32")
```

```
predictions = make_prediction(model, test)
```

```
submission_path = '/content/my_submission.csv'
df_submission = pd.DataFrame(columns=["Label"], data=predictions)
df_submission.index += 1
df_submission.to_csv(submission_path, index_label="ImageId")
```

```
!kaggle competitions submit -c digit-recognizer \
    -f {submission_path} -m "My submission"
```

## この先勉強をするために

この章の内容は、Chainer Colab Notebooks@<fn>{fn43}の一部を抜粋したものです。
//footnote[fn43][https://chainercv.readthedocs.io/en/stable/tutorial/index.html]

他にももっと基礎的なハンズオンや応用的なハンズオンも揃っていますので、ぜひ興味を持った方はお試しください。

//blankline

@<icon>{cheiko} 「こんなにあるんだからやるしかないね！」
