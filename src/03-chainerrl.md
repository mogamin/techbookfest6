# まえがき

うめざわ「ここでは、ChainerRLの説明をするよ」

ちぇいなーちゃん「CVは分かったけど、RLって何なのよ。東京駅で売ってるやつ？」

うめざわ「それは、ワッフルケーキだよね。RLは、Reinforcement Learningの略で、強化学習だよ。」

※ 本書は、東京駅で売っているカラフルなワッフルケーキ詰め合わせとは本当に何の関係もありません。

ちぇいなーちゃん「強化学習って？」

うめざわ「一般的に、機械学習って、教師あり学習・教師なし学習に分かれるよね。」

ちぇいなーちゃん「ふむふむ」

うめざわ「そうじゃないのが強化学習。それぞれこんな感じの特徴がある。」

* 教師あり学習
  * 入出力の関係を学習する（正解データが与えられる）
  * 一般的にイメージする機械学習（特にDeep learning）はこれが多い
* 教師なし学習
  * 正解データが与えられない
  * クラスタリング、異常検知など
* 強化学習
  * 試行錯誤を通じて価値を最大化する
  * 入力は乱数、正解データは与えられない（環境と報酬が与えられる）

ちぇいなーちゃん「そうじゃないのがって、ずいぶんとおざなりね。どういう時に使うの？」

うめざわ「そうだね、具体例から入った方が分かりやすいかな。強化学習で近年話題になったものに、Atariのゲームを人工知能で解いたっというのがあるんだ。DQNっていうんだけど、これにはゲームのルールを全く与えずに、繰り返しゲームをプレイするだけで、ブロック崩しとかがプレイできるようになったんだ」

![強化学習でブロック崩しを学習できる](src/images/chainerrl_fig1.png)

https://youtu.be/TmPfTpjtdgg

ちぇいなーちゃん「DQNって、、、」

うめざわ「そのDQNじゃないよ！Deep Q Networkの略だね。ゲームのルールを教えないけど、それがプレイできるようになるのってすごくない？」

ちぇいなーちゃん「すごいけど、画像認識だって、認識のルール教えてないわよ。同じじゃない？」

うめざわ「そうだね。でも画像認識は、正解があるよね。強化学習は、例えば、ゲームの画面と得点だけで、最適な行動を学習できる。もしこれを教師あり学習でやろうと思ったら、正解行動のデータが必要だけど、ブロック崩しみたいなゲームで正解行動のデータを作るとか難しいよね」

ちぇいなーちゃん「あー、なるほど。確かに、それができたらすごい気がしてきた。」

うめざわ「長々と話しちゃったけど、そんな感じのが強化学習。で、その強化学習を簡単にできるのが、ChainerRLなんだ」

ちぇいなーちゃん「やっと登場ね。」

そんなChainerRLですが、うめざわさんの言うとおり、強化学習を簡単に使えます。

ChainerRLの説明の前に、もう一つ、説明しなければならないものがあります。
うめざわさんも少し触れていましたが、強化学習には、エージェントと環境があります。

![強化学習の要素](src/images/chainerrl_fig2.png)

エージェントは、行動を決めるもので、ChainerRLではその内部で用意されています。
一方、環境は、シミュレーション環境等です。例えば、ブロック崩しの場合は、そのエミュレータが環境になります。
この環境とエージェントの間のI/Fが統一されていれば、エージェントのプログラムを変えずに、環境を入れ換えることができるはずです。

それを実現しているのがOpen AI Gymです。

手持ちの環境に、Open AI Gymを入れて見ましょう。

```
$ pip install gym
```

Open AI Gym は以下のようにして使う事が出来ます。

```
import gym
env = gym.make("Breakout-v0") # 1. 環境を作成
observation = env.reset() # 2. リセット。状態 s を受け取る
for _ in range(10):
  for _ in range(1000):
    env.render() # 3. 描画する。学習時は不要。
    action = env.action_space.sample() # 4. アクションを決定。ここではランダムサンプリング
    observation, reward, done, info = env.step(action) # ステップを実行し、状態、報酬、終了フラグを得る
    if done:
        break
```

上記のコードでは、ブロック崩しがゲームオーバーするかクリアするまで`env.step`を繰り返し読んでいます。
そしてそのゲームを10回繰り返します。
実行すると以下のような画面が表示されます。

![ブロック崩しの画面](src/images/chainerrl_fig3.png)

さて、では、これをChainerRLをつかって学習してみましょう。
ここでは、なるべくChainerRLに頼って、コードを書かずに書いてみます。
まず、ざっとコードを掲載します。本コードは、ChainerRLのQuickStartを参考にしています。

QuickStart
https://github.com/chainer/chainerrl/tree/master/examples/quickstart

```
import chainer
import chainerrl
import gym
import logging
import sys
import numpy as np


# 環境を作成
env = gym.make('CartPole-v0')

# 状態のサイズ
obs_size = env.observation_space.shape[0]
# 行動のサイズ
n_actions = env.action_space.n

# 離散行動用のQ関数を作成する
q_func = chainerrl.q_functions.FCStateQFunctionWithDiscreteAction(
    obs_size, n_actions,
    n_hidden_layers=2, n_hidden_channels=50)

# Adam を q_fuinc のオプティマイザに使う。安定のため eps=1e-2
optimizer = chainer.optimizers.Adam(eps=1e-2)
optimizer.setup(q_func)

# 将来の報酬の割引率を設定
gamma = 0.95

# Epsilon-greedy法を探索に使う
explorer = chainerrl.explorers.ConstantEpsilonGreedy(
    epsilon=0.3, random_action_func=env.action_space.sample)

# DQNでExperience Replayを使う
# Replay buffer を作成し、サイズを指定する
replay_buffer = chainerrl.replay_buffer.ReplayBuffer(capacity=10 ** 6)

# CartPole-v0 からの状態は numpy.float64 だが、Chainerは標準で
# numpy.float32 を受け付ける
# そのため、特徴抽出のphi関数として、型変換を定義する
phi = lambda x: x.astype(np.float32, copy=False)

# 環境とインタラクションするエージェントを作成する
agent = chainerrl.agents.DoubleDQN(
    q_func, optimizer, replay_buffer, gamma, explorer,
    replay_start_size=500, update_interval=1,
    target_update_interval=100, phi=phi)

# ログの設定
logging.basicConfig(level=logging.INFO, stream=sys.stdout, format='')

# 学習と評価を行う
# result/ に結果が保存される
chainerrl.experiments.train_agent_with_evaluation(
    agent, env,
    steps=20000,           # 20000ステップ学習する
    eval_n_runs=10,       ## 評価の度に10回実行する
    eval_interval=1000,   # 1000ステップ毎に評価する
    outdir='result')      # resultに出力
```

順に見てみましょう。

```
env = gym.make('CartPole-v0')
```

最初の辺りは、前述の通り、Open AI Gymの環境を作成しています。
ここでは、学習の早い、倒立振子（CartPole）を作成しています。

![倒立振子の画面](src/images/chainerrl_fig4.png)

```
obs_size = env.observation_space.shape[0]
n_actions = env.action_space.n
```

は、状態サイズと行動のサイズを取っています。それぞれ、先ほど作成した、envから取得できます。
倒立振子では、`obs_size=4`、`n_actions=2`となっています。
状態のサイズは4ですが、以下のような4つの変数になります。

* カートの位置 -2.4～2.4
* カートの速度 -3.0～3.0
* 棒の角度 -41.8～41.8
* 棒の角速度 -2.0～2.0

また、アクションは2つです。

* カートを右に移動
* カートを左に移動

```
q_func = chainerrl.q_functions.FCStateQFunctionWithDiscreteAction(
    obs_size, n_actions,
    n_hidden_layers=2, n_hidden_channels=50)
```

ここで、Q関数（Q-Function）というものを作成しています。Q関数とは、ある状態である行動をしたときの将来の報酬の総和を推定する関数です。つまり、環境がある状態にあるとき、どの行動が一番、お得な選択であるか、を推定する関数と言えるでしょう。この関数は、単純には、状態と行動のテーブルがあれば実装可能です。

![Q関数の実装例（表）](src/images/chainerrl_fig5.png)

しかし、倒立振子のようなシンプルなものでも、上記の表は巨大なものになってしまいます。
というのも、状態は単純な右か左か、と言うものですが、状態は、4つの連続値であるためです。
そのため、上記のテーブルは膨大な数になり、計算困難となります。

そこで、これをニューラルネットワークで近似しています。
ここで、`FCStateQFunctionWithDiscreteAction`という長い名前の意味を見てみると、以下の通りです。

* `FCState`は、Fullly Connected State。つまり、StateをFully Connected Layerのニューラルネットワークで近似します。
* `DiscreteAction`は、離散的な行動という意味です。倒立振子は、右に移動、左に移動と離散的な行動（つまり連続的な値を取らない）を取ります。

```
optimizer = chainer.optimizers.Adam(eps=1e-2)
optimizer.setup(q_func)
```

これは、機械学習でおなじみの最適化関数です。今回、Adamを使います。

```
gamma = 0.95
```

これは、将来報酬の割引率というものです。`gamma`という通り、以下の数式のγ(ガンマ)の係数です。0〜1の数字で、大きいほど将来の報酬を重視し、小さいほど直近の報酬しか見ないという設定になります。

![](src/images/chainerrl_fig6.png)

```
explorer = chainerrl.explorers.ConstantEpsilonGreedy(
    epsilon=0.3, random_action_func=env.action_space.sample)
```

これは、探索の設定をしています。Q関数を学習させるとき、常にその時点で最適なものを選択すると、もっと良い行動があったとしても、それを見つけられません。そのため、ある程度ランダムに行動して、より良い行動を探る必要があります。その探索の手法の一つに、Epsilon-Greedy法というのがあります。この手法では、1-εの確立でランダムな値を選ぶ事で探索を行います。

```
replay_buffer = chainerrl.replay_buffer.ReplayBuffer(capacity=10 ** 6)
```

これは、Experience Replay Bufferの設定をしています。Experience Replay Bufferとは、過去に経験した状態や行動のデータを保存し、後でランダムに抜き出して学習する手法です。

```
phi = lambda x: x.astype(np.float32, copy=False)
```

これは特徴抽出に用いるphi関数というものです。といっても、ニューラルネットワークで近似するためここで特徴抽出をする必要は無く、型変換をしているだけです。倒立振子 からの状態は numpy.float64 で表現されていますが、Chainerは標準ではnumpy.float32 を受け付けるため、型変換をしています。

```
agent = chainerrl.agents.DoubleDQN(
    q_func, optimizer, replay_buffer, gamma, explorer,
    replay_start_size=500, update_interval=1,
    target_update_interval=100, phi=phi)
```

やっとエージェントの作成にたどり着きました。ここでは、ここまで作ったものを与えて、エージェントを作っています。

```
chainerrl.experiments.train_agent_with_evaluation(
    agent, env,
    steps=20000,           # 20000ステップ学習する
    eval_n_runs=10,       ## 評価の度に10回実行する
    eval_interval=1000,   # 1000ステップ毎に評価する
    outdir='result')      # resultに出力
```

最後に、`train_agent_with_evaluation`を使って学習と評価をします。

では、このコードを実行してみましょう。概ね10分ほどで終わると思います
すると、result以下に、scores.txtが作成されます。

```
steps	episodes	elapsed	mean	median	stdev	max	min	average_q	average_loss	n_updates
1013	52	3.7664895057678223	9.358	9.0	0.7529740632184879	11.0	8.0	7.567612714092402	0.2999092669597402	513
2006	128	11.854687690734863	24.78	25.0	4.3484961581764505	35.0	16.0	12.073888504675862	0.6073129824974002	1506
3038	142	27.868073225021362	68.164	67.0	10.273470318483893	113.0	46.0	15.69104218384753	0.4823906248083028	2538
4
...
```

mean、meadian、stdev、max、minは、評価の時に得られた報酬の平均・中央値・標準偏差・最大値・最小値を示しています。学習が進むにつれて、meanが増えているのが分かると思います。

では、学習結果を確認してみましょう。最後の`train_agent_with_evaluation`の行を以下の通り置き換えて実行してください。

```
agent.load('result/20000_finish')
env = chainerrl.wrappers.Render(env)
eval_stats = chainerrl.experiments.eval_performance(
    env=env,
    agent=agent,
    n_runs=1000)
```

20000ステップでは学習が少ないので、もう少しという所ですが、倒立振子がそれなりに安定して居るのが確認できたかと思います。

以上のように、ChainerRL は強化学習の難しいところをとてもうまく隠蔽して、使いやすくしているのが分かるかと思います。ChainerRLのサンプルには、ゲームやロボットの例があるので色々と実行してみると面白いと思います。