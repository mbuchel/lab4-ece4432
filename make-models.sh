curl -LO https://github.com/mozilla/DeepSpeech/releases/download/v0.5.1/deepspeech-0.5.1-models.tar.gz
tar xvf deepspeech-0.5.1-models.tar.gz
mkdir models
mv deepspeech-0.5.1-models/* models/
rm -rf ._*
rm -rf deepspeech*
