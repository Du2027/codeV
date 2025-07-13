#include <cstdio>
#include <cstdlib>
#include <future>
#include <string.h>
#include <vector>

#define BUFFERWIDTH 5000

enum DownloadSite { SPOTIFY = 1,
                    YOUTUBE = 0 };

int download(char *link, int linkSize, DownloadSite site) {
  char tmp[linkSize + 10];
  switch (site) {
  case SPOTIFY:
    strcpy(tmp, "spotdl '");
    break;
  case YOUTUBE:
    strcpy(tmp, "yt-dlp '");
    break;
  }

  strcat(tmp, link);
  strcat(tmp, "'");
  tmp[linkSize + 10] = 0;

  return system(tmp);
}

int main(int argc, char **argv) {
  std::vector<char> link = {};
  short threadCount = 4;
  short currentThread = 0;
  std::future<int> results[std::thread::hardware_concurrency()];

  if (system("yt-dlp --version") != 0) {
    printf("yt-dlp isnt installed, see https://github.com/yt-dlp/yt-dlp for installation!\n");
    return -1;
  }
  if (system("spotdl --version") != 0) {
    printf("spotDl isnt installed, see https://github.com/spotDL/spotify-downloader for installation!\n");
    return -1;
  }

  char toDownloadLink[BUFFERWIDTH] = {0};
  if (argc == 1) { // Works, but looks very wierd with output of the command
    while (true) {
      // input and parsing the link
      printf("Enter a Link\n");
      scanf("%s", &toDownloadLink);

      DownloadSite site;
      if (toDownloadLink[13] == 's') {
        site = SPOTIFY;
        printf("Downloading from Spotify...\n");
      } else {
        site = YOUTUBE;
        printf("Downloading from Others...\n");
      }

      // get right size of the link
      int i = 0;
      char nextChar = toDownloadLink[0];
      while (nextChar != 0) {
        if (i == BUFFERWIDTH - 1) {
          printf("BUFFERERR\n");
          return -1;
        } else {
          nextChar = toDownloadLink[i];
        }
        i++;
      }

      // asyncronusly download
      results[currentThread] = std::async(std::launch::async, download, toDownloadLink, i, site);
      link.clear();

      currentThread++;
      if (currentThread == threadCount) {
        results[0].wait();
        currentThread = 0;
      }
    }
  } else {
    // iterate for each arg
    for (int n = 1; n < argc; n++) {
      char toDownloadLink[5000] = {0};
      strcpy(toDownloadLink, argv[n]);

      DownloadSite site;
      if (toDownloadLink[13] == 's') {
        site = SPOTIFY;
        printf("Downloading from Spotify...\n");
      } else {
        site = YOUTUBE;
        printf("Downloading from Others...\n");
      }

      // get right size of the link
      int i = 0;
      char nextChar = toDownloadLink[0];
      while (nextChar != 0) {
        if (i == BUFFERWIDTH - 1) {
          printf("BUFFERERR\n");
          return -1;
        } else {
          nextChar = toDownloadLink[i];
        }
        i++;
      }

      // asyncronusly download
      results[currentThread] = std::async(std::launch::async, download, toDownloadLink, i, site);
      link.clear();

      currentThread++;
      if (currentThread == threadCount) {
        results[0].wait();
        currentThread = 0;
      }
    }
  }
  return 0;
}
