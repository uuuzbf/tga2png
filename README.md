## tga2png
This program converts all .tga files in the current directory to .png files, and strips out the transparency channel. Files where the target .png already exists are skipped. File modification times are also copied so you can sort by file date.

## Usage with Battlefield 1942 screenshots
Copy this tga2png into your `ScreenShots` folder and run it. It will convert all .tga files to .png.

### Why is this needed?
By default the game saves images in .tga files, which is not supported by Windows's image viewer (?). Also the images usually are completely transparent, so even if you open it in another image viewer you won't see anything. This is why the alpha channel is stripped from the image.

## Compiling
You can compile in VS command prompt with the following:
```
cl /std:c++17 /O2 tga2png.cpp
```

With g++:
```
g++ -std=c++17 -O2 -o tga2png tga2png.cpp
```

## About the source
This program was completely made by ChatGPT, you can see the session here: https://chatgpt.com/share/17e3114c-dfab-412f-acaa-d806e446232e
