# qytlib

A YouTube library for Qt, based on [YouTubeExplode](https://github.com/Tyrrrz/YoutubeExplode) 5.0.5.

## Usage

### Getting metadata of a video

```c++
auto *networkManager = new QNetworkAccessManager;
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
- [ ] Videos
	- Todo: Closed Captions