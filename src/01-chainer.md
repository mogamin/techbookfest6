# Chainerを取り巻くエコシステム

こんにちは、Chainer開発チームの前橋です。
本章では、ChainerとChainerを取り巻くエコシステムの最新情報について改めて整理してご紹介したいと思います。

## Chainerのリリースポリシー

現在Chainerチームでは、原則として毎月1回、開発版と安定版のリリースを行っています[^11]。
[^11]: 本節のリリースポリシーはCuPyについても同様です。
具体的な最近のリリースの状況は次の通りです。

|    | 2018/03   | 2018/04  | 2018/05  | 2018/06  | 2018/07               | 2018/08  |
|----|-----------|----------|----------|----------|-----------------------|----------|
| v6 |           |          |          |          |                       |          |
| v5 |           | v5.0.0a1 | v5.0.0b1 | v5.0.0b2 | v5.0.0b3              | v5.0.0b4 |
| v4 | v4.0.0rc1 | v4.0.0   | v4.1.0   | v4.2.0   | v4.3.0 @<br>{} v4.3.1 | v4.4.0   |

|    | 2018/09   | 2018/10  | 2018/11  | 2018/12 | 2019/01  | 2019/02  |
|----|-----------|----------|----------|---------|----------|----------|
| v6 |           | v6.0.0a1 | v6.0.0b1 |         | v6.0.0b2 | v6.0.0b3 |
| v5 | v5.0.0rc1 | v5.0.0   | v5.1.0   |         | v5.2.0   | v5.3.0   |
| v4 | v4.5.0    |          |          |         |          |          |

開発版(α,β,RC)と安定版の大きな違いは、マイナーアップデート(X.Y.ZのYのインクリメント)における新機能の追加の有無にあります。
開発版のマイナーアップデートでは、新機能を含む様々な改善が随時追加されてゆき、予定されていた機能の実装が完了するとリリース候補(RC)版の公開を経て次の安定版リリース(X.0.0)へと移行してゆきます。
一方、安定版のマイナーアップデートでは、開発版にマージされた修正のうちパフォーマンスの向上やバグフイックスなどAPIの追加や破壊的変更を伴わない改善のみがバックポートされてゆきます。
開発版も安定版も基本的には同等のテストプロセスを経てリリースされますが、開発版ではマイナーアップデートの過程で社内外からのフィードバックに基づくAPIの見直しなど影響の大きな修正が行われることがあります。
このように二つの異なるブランチでリリースを行うことで、新機能を積極的に必要とするユーザ(リサーチャー等)と、後方互換性の保証されたAPIを必要とするユーザ(アプリケーション開発者等)の双方のニーズを満たせるようにしています。

また、毎月リリースを行うことによって常に最新のハードウェア支援技術を利用できることもメリットの一つです。
CUDA Toolkit、cuDNN、iDeepといったハードウェアアクセラレータを活用するためのライブラリは、Chainerのエコシステムを構成する重要なソフトウェアスタックです。
これらのライブラリは各ベンダによって常に改善が続けられているため、最新版へアップデートすることによってパフォーマンスの改善が期待できます。
例えば、あるCNNのユースケースでは、同一ハードウェア(NVIDIA Tesla V100)上でcuDNN 7.0と7.4の性能を比較すると、20%程度スループットが向上していることが示されています[^12]。
[^12]: https://developer.nvidia.com/cudnn
新しいバージョンのライブラリがリリースされると、開発版・安定版ともに原則として直近のリリース(タイミングによってはその次のリリース)で対応作業が行われます。
このため、Chainerのパフォーマンス・ベストプラクティス[^13]では、第一に最新のChainer/CuPyと最新版のライブラリを利用することを推奨しています。
[^13]: https://docs.chainer.org/en/latest/performance.html
Chainer/CuPyや、各ライブラリを最新バージョンに更新することで、機能性だけでなく性能向上や最新のハードウェアサポートといったエコシステムのメリットを最大限享受することができるようになります。
なお、CuPyの公式バイナリパッケージには、その時点でサポートされている最新のcuDNNライブラリが同梱されているため、ユーザは特段意識することなくCuPyをアップデートするだけで常に最新のcuDNNを利用することができるようになっています。

一方で、Chainerはアップデートが頻繁なのでキャッチアップが大変、という印象をお持ちの方もいらっしゃるかもしれません。
既に述べたように、Chainerの安定版ではAPIの後方互換性を保証しているため、原則としてマイナーアップデート時にユーザ側でコードの修正を行う必要はありません[^14]。
[^14]: より正確に述べると、ChainerではAPIリファレンスに掲載されている関数やクラスを公開APIと位置付け、マイナーアップデート時の後方互換性を保証しています。ただ、実態としてPython界隈ではドキュメントだけでなくライブラリのソースコードを読んでいるユーザーも多いと思われることから、非公開APIであってもユーザーが使用している可能性が高いと思われる関数やクラスについては可能な限り互換性を維持するようにしています。
メジャーアップデート(X.Y.ZのXのインクリメント)についてはAPIの変更が生じる場合もありますが、影響があると考えられる変更点はアップグレードガイドにまとめられています[^15]。
[^15]: https://docs.chainer.org/en/latest/upgrade.html
アップグレードガイドを見ていただければ分かる通り、Chainer v2からv6までのメジャーアップデート間では、後方互換性を大きく損なうようなAPIの変更はほとんど行われていません[^16]。
[^16]: Chainerチームでは、品質保証のプロセスにおいてChainer v2向けに書かれたExampleコードが開発版のChainerで実行できることを常に確認しています。
ぜひ最新版のChainerを積極的に活用していただければと思います。

## Chainerを支えるソフトウェア

2015年のChainerリリース以来、Chainerを利用した研究開発を加速する様々な関連プロダクトが開発・公開されてきました。
本節ではChainerを取り巻くソフトウェア環境についてご紹介します。

### Chainerファミリー

ご存知の方も多いと思いますが、PFNではChainerファミリーと呼ばれるライブラリ群を公開しています。
ChainerCV(コンピュータビジョン)、ChainerRL(強化学習)、Chainer Chemistry(化学・生物学)といったドメイン特化型のライブラリでは、論文の再現実装や学習済みの重みデータのほか、各ドメインのタスクを実装する際に有用な共通機能(データセットの抽象化など)が提供されており、SoTAの再現実験、ファインチューニング、スクラッチでのモデル記述などを効率的に行うことができます。
また、ChainerMN[^21]によるマルチノード/マルチGPUを活用した分散深層学習、ChainerUIによる実験結果のWebブラウザ上での可視化など、深層学習の研究現場で実際に生じたニーズから生まれた幅広いライブラリが揃っていることはChainerの強みの一つです。
[^21]: Chainer v5以降、ChainerMNはChainer本体に統合されています。

### 推論のためのソフトウェアスタック

Deep Learningの実用化が進むにつれ、フレームワークに求められる役割は実験効率だけでなく推論やデプロイの容易さへと広がりました。
Chainerの特徴であるDefine-by-Runによる柔軟性はそのままに、効率的でインテグレーションの容易な推論を実現するためのソフトウェアスタックとして、ONNX、TensorRT、ChainerX、Chainer Compilerの4つをご紹介します。

#### ONNX

ONNX (Open Neural Network eXchange, オニキス)[^22]はDNNのネットワーク構造と重み情報をフレームワーク間で相互に交換するためのファイルフォーマットです。
Chainerでは `onnx-chainer` というパッケージ[^23]を利用することで、既存のChainerモデルをONNXフォーマットでエクスポートすることができます。
[^22]: https://onnx.ai/
[^23]: https://github.com/chainer/onnx-chainer

`onnx-chainer` の使用方法は以下のようにシンプルです。

```py
import onnx_chainer

# model (Chainerのモデル) に入力 x (ダミーデータ) を与えて推論し、
# 得られた計算グラフを ONNX フォーマットのファイルとして保存する
onnx_chainer.export(model, x, filename='output.onnx')
```

ONNXフォーマットに変換したモデルファイルは、ONNXフォーマットの読み込みをサポートする他のフレームワークで実行できるため、Chainerで構築したモデルをスマートフォンやエッジデバイスを含む様々な環境にデプロイすることが可能となります。
PFNの開発しているDNN推論ライブラリであるMenohフレームワークも、デプロイ時に利用できる選択肢の一つです。
MenohにはC/C++/C#/Ruby/Java/Go/Node.js/Haskell/Rustといった多様な言語のAPI(バインディング)が用意されているため、これらのプログラミング言語で記述されたアプリケーションに推論機能を容易に組み込むことができます。
現在MenohではMKL-DNN[^24]を利用したIntelアーキテクチャ上での推論のみがサポートされていますが、TensorRT(後述)、Arm NN[^25]、ONNXIFI[^26]など他のバックエンドの開発も進行しています。

[^24]: Math Kernel Library for Deep Neural Networks; Intelアーキテクチャ上でDNNアプリケーションを高速化するライブラリ。 @<br>{} https://github.com/intel/mkl-dnn
[^25]: Armアーキテクチャに最適化されたDNN推論エンジン。 @<br>{} https://developer.arm.com/ip-products/processors/machine-learning/arm-nn
[^26]: ONNX Interface for Framework Integration; ONNXをサポートするDNN推論エンジンの共通API仕様。 @<br>{} https://github.com/onnx/onnx/blob/master/docs/ONNXIFI.md

#### TensorRT

TensorRT[^27]はNVIDIA社が開発するDNN推論ライブラリで、計算グラフの事前最適化により低レイテンシ・高スループットな推論が行えることが特徴です。
[^27]: https://developer.nvidia.com/tensorrt
`chainer-trt` [^28]を利用することで、既存のChainerモデルをTensorRTで実行可能な推論器に変換することが可能となっています。
[^28]: https://github.com/pfnet-research/chainer-trt, https://research.preferred.jp/2018/12/chainer-trt/

#### ChainerX

現在開発版が公開されているChainer v6には、ChainerX[^29]と呼ばれる新しい行列演算ライブラリが含まれています。
[^29]: https://chainer.org/announcement/2018/12/03/chainerx.html
ChainerXはC++で記述された行列演算・自動微分機能と、それらを既存のChainerコードから呼び出すためのPython API(ラッパー)で構成されています。
ChainerXのバックエンドはプラガブルになっており、現在はNativeバックエンド(CPU)およびCUDAバックエンド(GPU)が提供されています。

現在公開APIとなっているのはPython API[^30]のみですが、将来的にC++ APIの公開を予定しています。
[^30]: https://docs.chainer.org/en/latest/chainerx/reference/
これにより、

* 検証フェーズでは、Python APIとGPUを利用してフレキシブルに実験を行う
* プロダクションフェーズでは、C++ APIとCPUを利用してPythonランタイムを必要としない推論アプリケーションを実装する

といった使い分けを一つのフレームワークで完結して行うことができるようになります。

#### Chainer Compiler

実験的な取り組みとして、Chainer Compiler[^31]と呼ばれるコンパイラの開発を進めています。
[^31]: https://github.com/pfnet-research/chainer-compiler
`onnx-chainer` とは異なり、Pythonの構文木を直接解釈して計算グラフを取り出し、最適化を適用してからChainerX上で実行することで、Pythonの柔軟性と高い実行効率を同時に実現することが目標となっています。

### 対応プラットフォームの拡充

Chainer/CuPyを実行するプラットフォームのサポートも拡充しています。
その一つが、Windowsへの対応です。
CuPy v5以降ではWindows用のバイナリパッケージを公式で提供しており、Visual Studioなどのコンパイル環境を用意することなくインストールすることが可能となっています。

また、クラウドサービス上でもChainer/CuPyを容易に利用できる環境が整っています。
Amazon Web Services (AWS)の深層学習AMI[^32]、Google Compute Engine (GCE)のDeep Learning VM[^33]やGoogle Colaboratory[^34]、Microsoft AzureのAzure Data Science Virtual Machines[^35]などではChainer/CuPyがデフォルトで利用可能となっています。
[^32]: https://aws.amazon.com/jp/machine-learning/amis/
[^33]: https://cloud.google.com/deep-learning-vm/docs/images
[^34]: CUDA GPUが利用できるクラウド型のJupyter Notebook環境で、Googleアカウントを保有していれば無償で利用することが可能です。 @<br>{} https://colab.research.google.com/
[^35]: https://azure.microsoft.com/ja-jp/services/virtual-machines/data-science-virtual-machines/

## 再現実装やドキュメントなどの資産

深層学習フレームワークでは、その機能性もさることながら、フレームワークに関する情報の入手性(再現実装やドキュメントが充実していること)も重要な魅力の一つとなっています。

そこで、Chainerプロジェクトでは、様々な論文の再現実装を一元的に公開するChainer Modelsリポジトリ[^41]を昨年度から運用しています。
[^41]: https://github.com/chainer/models
また、chugでは有志によって公開された再現実装のリポジトリをまとめたAwesome Chainer[^42]というリンク集を公開しています。
[^42]: https://github.com/chainer-community/awesome-chainer
再現実装を探している場合は、まずこちらの2つに目を通していただくとよいでしょう。
これらのリポジトリでは外部からのコントリビューションも歓迎していますので、再現実装を公開する場合はPull-Requestを検討していただければ幸いです。

また、日本語で利用可能な教材の充実にも力を入れています。
現時点でもっともお勧めできる初学者向けの資料はメディカルAI専門コースのオンライン講義資料[^43]です。
[^43]: https://japan-medical-ai.github.io/medical-ai-course-materials/
数学の基礎やDeep Learningフレームワークの使い方といった基本的な知識から、セグメンテーションや時系列解析といった実践的なトピックまで幅広くカバーしています。
また、chugでもChainerのサンプルコードをJupyter Notebook化したChainer Colab Notebooks[^44]を公開しています。
[^44]: https://chainer-colab-notebook.readthedocs.io/ja/latest/
これらはいずれも先述したGoogle Colaboratoryで実行可能なチュートリアルとなっており、GPUなどのハードウェアをお持ちでなくても、実際に手を動かしながらChainerの利用方法を習得することができるようになっています。

今後も再現実装やドキュメントといった資料の充実化を継続的に進めてゆく予定です。
