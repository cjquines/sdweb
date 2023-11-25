# sdweb

tested with emcc version 3.1.48

the sd folder should be a near verbatim copy of sd source, except for:
- `.clang-format`
- `sdweb/`

to build:
- `cd build` and run `emmake make`
- `cd ../ui` and run `npm run build`
- serve from `dist` folder
