# qytlib

![CI (GitHub)](https://github.com/PhilInTheGaps/qytlib/workflows/CI/badge.svg)
[![CI (Travis)](https://travis-ci.com/PhilInTheGaps/qytlib.svg?branch=master)](https://travis-ci.com/PhilInTheGaps/qytlib)
[![Coverage](https://coveralls.io/repos/github/PhilInTheGaps/qytlib/badge.svg?branch=master)](https://coveralls.io/github/PhilInTheGaps/qytlib?branch=master)

A YouTube library for Qt, based on [YouTubeExplode](https://github.com/Tyrrrz/YoutubeExplode) 5.0.5.

## Usage

### Getting metadata of a video

```c++
#include <qytlib/videos/videoclient.h>

// ...

auto *networkManager = new QNetworkAccessManager;
networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

auto *client = new YouTube::Videos::VideoClient(manager);

// Video ID or URL
auto *video = client->getVideo("dQw4w9WgXcQ");

connect(video, &Videos::Video::ready, this, [=]() {
    auto title = video->title();
    auto author = video->author();
    auto description = video->description();

    video->deleteLater();
});
```

## Progress

How much of the YoutubeExplode library is implemented in qytlib.

- [ ] Channels
- [x] Common
- [ ] Internal
	- Solution for segmented http stream?
- [ ] Playlists
- [ ] ReverseEngineering
	- Todo: Responses for Playlist and Channel
- [ ] Search
- [x] Videos
