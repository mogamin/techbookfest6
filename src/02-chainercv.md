# chainercv は魔法のツール！

@<icon>{yousei} 「Deep Learningの花形といえば、なんといっても画像分類だね。でもってChainerには画像の処理を簡単に実装できちゃう機能がたくさんあることは当然知っているよね？」

　

@<icon>{cheiko} 「も、もちろんよ・・・」

　

@<icon>{yousei} 「じゃあ、さっそくやってみよう。」

　

　せんせいの言う通りchianercvには、簡単に使える画像処理の機能がたくさん用意されています。しかもとっても簡単に使えるのです。さっそく画像処理の基礎からやってみましょう。なにはともあれchainercvをインストール！


```
!pip install chainercv
```

この章ではGoogleColabratory@<fn>{fn01}上で実行することを想定しています。
//footnote[fn01][https://colab.research.google.com/]


## 基本的な操作


まずは画像の基本的な操作として画像の読み込みと表示をやってみましょう。今回はchainer user group のロゴ@<fn>{fn02}を用意したのでダウンロードして使います。
//footnote[fn02][https://avatars1.githubusercontent.com/u/20376886?s=200&v=4]


```
!curl "https://avatars1.githubusercontent.com/u/20376886?s=200&v=4" \
  -o chainercv_fig01.png
```

![chug logo](src/images/chainercv_fig01.png)

このロゴは縦と横がそれぞれ200pxのRGB画像です。この画像を使って次のコードを実行してみましょう。

```
import numpy as np
from chainercv.utils.image import read_image

image = read_image("./chainercv_fig01.png")
print(image.shape)
```

`(3, 200, 200) ` と出力されましたね@<fn>{fn03}。これは 縦200px 横200px の面が３つあることを示しています。これは「３チャネルある」と言って、次の図のようなイメージで構成されています。チャネルの順番は0番目がR（赤）、1番目がG（緑）、2番目がB（青）となっていて、それぞれ各色の濃淡の情報が格納されているのです。
//footnote[fn03][その他の画像処理系のライブラリでは(200, 200, 3)のようにチャネルの次元を示す位置が異なる場合があるので注意！]


![chainercv fig02](src/images/chainercv_fig02.png)


３つのチャネルの情報が重なって多くの色を表現することができるのです。次に、それぞれのチャネルだけの単色画像を表示させてみましょう。先程のコードの続きで実行してみてください。

```
from chainercv.utils.image import tile_images
from chainercv.visualizations import vis_image

image_R = np.copy(image)
image_R[(1, 2), :, :] = 0     # GとBのチャネルの情報をすべて0にする
image_G = np.copy(image)
image_G[(0, 2), :, :] = 0     # RとBのチャネルの情報をすべて0にする
image_B = np.copy(image)
image_B[(0, 1), :, :] = 0     # RとGのチャネルの情報をすべて0にする

# タイルイメージとして作成し表示する
images = np.array([image_R, image_G, image_B])
tile_image = tile_images(images, 3)
vis_image(tile_image)
```

![chainercv fig03](src/images/chainercv_fig03.png)


上記のように表示されます。
例えば、左端は赤を表すチャネルの単色画像で、ロゴがうっすらと見えるかと思います。これは普通の赤よりも濃い赤が使用されていることを表しています。そして、中と右端の画像ではロゴが真っ黒になっています。これは、ロゴの部分に緑と青の要素がまったくないことを表しています。

これらは混ざると明るくなり白に近づいていく混色方法であり、光の３原色と呼ばれています。

　

次に、もうちょっとだけわかりやすい例をやってましょう。ロゴの色を濃い赤ではなく「普通の赤」となるようにしてみましょう。先程のコードの続きで実行してみてください。

```
image_realR = np.copy(image_R)
image_realR[(0), :, :]    = 255   # Rのチャネルの情報をすべて255にする
image_realR[(1, 2), :, :] = 0     # G,Bのチャネルの情報をすべて0にする

# RGBの単勝画像をタイルイメージを作成
images = np.array([image_realR, image_G, image_B])
tile_image = tile_images(images, 3)
vis_image(tile_image)

# オリジナル画像と、Rチャネルをすべて255にしたタイルイメージを作成
red_image = np.array([image_realR[0], image_G[1], image_B[2]])
vis_image(red_image)
```


//image[chainercv_fig04][chainercv fig04][scale=0.8]{
//}


いかがでしたでしょうか？
この章では、chainercvのライブラリを使って、画像の読み込み、RGBのチャネル情報の分解を経て、画像の表示まで勉強しました。

　

chainercvを使って読み込んだ画像の情報は、３つの色チャネルで構成され、縦横で表される領域には0～255の値がそのチャネルの色の強さを示していることが理解できましたね。これらはchainercvに限らず画像処理の基本中の基本なので抑えておくとよいでしょう。


## 画像の変形

この章では画像のリサイズ、反転、回転、スケールを学びます。それぞれ非常に簡単で、直感的なコードなのです。是非試してみてください。


### リサイズ

リサイズとは画像サイズの変更です。つまり拡大縮小ができます。先程のコードの続きで実行してみてください。

```
from chainercv.transforms.image.resize import resize

# (200px,200px)を(100px,300px)へサイズを変更する
resized_image = resize(image, (100,300))
vis_image(resized_image)
```

![chainercv fig10](src/images/chainercv_fig10.png)

簡単ですね。

　

### 反転

上下反転、左右反転ができます。先程のコードの続きで実行してみてください。


```
from chainercv.transforms.image.flip import flip

fliped_image1 = flip(image,  y_flip=True, x_flip=False) # 上下反転
fliped_image2 = flip(image,  y_flip=False, x_flip=True) # 左右反転

images = np.array([fliped_image1, fliped_image2])
tile_image = tile_images(images, 2)
vis_image(tile_image)
```

![chainercv fig11](src/images/chainercv_fig11.png)

これも簡単ですね。ただ右側は左右反転させているのですが、ロゴが左右対称なので違いがわからないですね。

　

### 回転

今度は回転です。角度を指定します。先程のコードの続きで実行してみてください。


```
from chainercv.transforms.image.rotate import rotate

rotated_image1 = rotate(image,  45) # 45度回転
rotated_image2 = rotate(image,  90) # 90度回転

images = np.array(
    [resize(rotated_image1, (100,100)), 
     resize(rotated_image2, (100,100))
    ]
)
tile_image = tile_images(images, 2)
vis_image(tile_image)
```

![chainercv fig12](src/images/chainercv_fig12.png)

どれも簡単でしたね。


## 画像の水増しとは

Deep Learningの精度は質の高い画像の量に比例すると言われています。だからといって、いろんな形や角度の写真を撮影するのは現実的ではありません。そこでDeep Learningでは画像を水増し(data augmentation)するという手法を取ることが一般的なのです。これにより実際には撮影していないけど、あたかも撮影したかのように仮想的な画像を生成してニューラルネットに学習させることができます。では水増しの手法をいくつか見ていきましょう。



### random_crop

指定されたサイズの画像を元画像からランダムで切り出します。先程のコードの続きで実行してみてください。


```
from chainercv.transforms.image.random_crop import random_crop

random_images = []

for x in range(5):
  # (100,100)のサイズでランダムで画像を切り出す
  _image = random_crop(image, (100, 100))
  random_images.append(_image)

# list型をNumPy配列ndarrayに変換しタイルイメージとして作成
images = np.array(random_images)
tile_image = tile_images(images, len(random_images))
vis_image(tile_image)
```

![chainercv fig20](src/images/chainercv_fig20.png)


　

### random_flip

これは元画像をランダムに反転させます。先程のコードの続きで実行してみてください。


```
from chainercv.transforms.image.random_flip import random_flip

random_images = []

for x in range(5):
  # ランダムで画像を反転させる
  _image = random_flip(image, y_random=True, x_random=True)
  random_images.append(_image)

# list型をNumPy配列ndarrayに変換しタイルイメージとして作成
images = np.array(random_images)
tile_image = tile_images(images, len(random_images))
vis_image(tile_image)
```

![chainercv fig21](src/images/chainercv_fig21.png)

左右反転が無いように見えるのは、ロゴが左右対象だからですね。
角度を0度、45度、90度、180度、270度でランダムで回転させるrandom_rotateもあります。

　

### random_expand

これは元画像をランダムに拡大・縮小させます。先程のコードの続きで実行してみてください。


```
from chainercv.transforms.image.random_expand import random_expand

random_images = []

for x in range(5):
  # ランダムで画像を拡大・縮小させる
  _image = random_expand(image)
  _image = resize(_image, (200,200))
  random_images.append(_image)

# list型をNumPy配列ndarrayに変換しタイルイメージとして作成
images = np.array(random_images)
tile_image = tile_images(images, len(random_images))
vis_image(tile_image)
```

![chainercv fig22](src/images/chainercv_fig22.png)


　
### 全部まとめてやってみる

これまでに紹介したrandom_crop,random_flip,random_rotate,random_expandをランダムにやってみるとどうなるでしょう。先程のコードの続きで実行してみてください。


```
import random

T = True
T_OR_F = [True, False]
random_images = []

for x in range(20):
  # 各種ランダム系の画像処理をランダムで実行する
  _image = image
  if random.choice(T_OR_F) == T: _image = random_crop(_image, (100, 100))
  if random.choice(T_OR_F) == T: _image = random_rotate(_image)
  if random.choice(T_OR_F) == T: _image = random_flip(_image, y_random=True)
  if random.choice(T_OR_F) == T: _image = random_expand(_image)
  random_images.append( resize(_image, (200,200)) )

# list型をNumPy配列ndarrayに変換しタイルイメージとして作成
images = np.array(random_images)
tile_image = tile_images(images, 5)
vis_image(tile_image)
```

![chainercv fig23](src/images/chainercv_fig23.png)


１枚の画像からこれだけの画像データを簡単に生成できるのはchainercvだからですね。これなら質の高い画像を簡単に大量に作ることができます。


　

## データセットの取得

前章でもあったように、Deep Learningでは学習に大量の画像が必要となります。Googleで画像検索して一つ一つ集めるのは大変ですよね？まさかスクレイピングのプログラムを書きます？いやいや。どれもDeep Learningとは直接関係のない作業に多くの時間が費やされてしまいます。これではせっかくの勉強意欲も半減です。

　

そこでchainercvです。chainercvには画像処理の機能以外にも大量の画像セットを数行で取得できる簡単な仕組みを備えています。それではさっそく試してみましょう。

　

取得できるデータセットはいくつかありますが、ここではVOC@<fn>{fn04}のデータセットを取得してみましょう。次のコードを実行するだけです。これだけで約5,000個の以上の質の高いデータを得られるのは本当に簡単ですね。先程のコードの続きで実行してみてください。
//footnote[fn04][PASCAL VOC アノテーション付きの画像データセット。2005-2012年まで精度を競うコンペティションのためのデータセット]


```
from chainercv.datasets.voc.voc_bbox_dataset import VOCBboxDataset

dataset = VOCBboxDataset()

images = []
for i in range(10):
  _data = dataset.get_example(i)
  _image = resize(_data[0], (200,200))
  images.append(_image)
  
tile_image = tile_images(np.array(images), 5)
vis_image(tile_image)
```

![chainercv fig30](src/images/chainercv_fig30.png)


他にもcoco@<fn>{fn05}、cityspace@<fn>{fn06}等々、Deep Learning界隈で有名なデータセットが簡単に取得できるように用意されています。いろいろと試して使ってみてくださいね。
//footnote[fn05][Common Object in Context, Microsoftが提供している画像の認識、セグメンテーション、キャプショニングがされているデータセット]
//footnote[fn06][ドイツのダイムラー社、マックス・プランク研究所、ダルムシュタット工科大学のチームが公開しているデータセット]



## この先勉強をするために

ChainerはDeep Learningのフレームワークですが、chainercvは画像処理のライブラリとして使っても十分な機能と直感的な使いやすさを持ち合わせています。もっと勉強したい人は豊富なチュートリアル@<fn>{fn07}がありますので、ちぇい子と一緒に進めてみましょう。
//footnote[fn07][https://chainercv.readthedocs.io/en/stable/tutorial/index.html]

　
　
　

@<icon>{yousei} 「もちろん英語版だけど、大丈夫だよね？」

　

@<icon>{cheiko} 「も、もちろんよ・・・」
　






