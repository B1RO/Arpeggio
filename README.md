# ARpeggio - AR Piano Teacher
**ARpeggio** is an Augmented Reality Application that displays notes on piano keys and aims to help piano students to learn songs.
It was created during the project phase of the lecture *Erweiterte Realität / Augmented Reality (IN2018)* at TUM by [@miruna20](https://github.com/miruna20), [@B1RO](https://github.com/B1RO), [@tobihanl](https://github.com/tobihanl) and [@goldst](https://github.com/goldst).

## Project status
This project should be seen more as a proof of concept than an actual program to teach students. The current features include displaying information on white keys in an area that is delimited by two markers on C notes. Black key support is implemented partially, but is not functional yet.

## External libraries and references
Name | Notes | License
-----|-------|--------
[libfft](https://github.com/bejayoharen/guitartuner/blob/master/src/libfft.c) | (C) 1989 by Jef Poskanzer. <br/> Included in the repository at `/music/libfft`. | BSD 
[guitartuner](https://github.com/bejayoharen/guitartuner) | (C) 2012 by Bjorn Roche. <br/> The code in `/music/MicPitchParser.cpp` is partially based on that project. | [BSD](https://github.com/bejayoharen/guitartuner#Copyright)
[midifile](https://github.com/craigsapp/midifile) | (C) 1999-2018, Craig Stuart Sapp. <br/> Partially included in the repository at `/music/midifile` | [BSD 2-Clause](https://github.com/craigsapp/midifile/blob/master/LICENSE.txt)
[PortAudio](http://portaudio.com/) | (C) 1999-2011 Ross Bencina and Phil Burk. <br/> By default, lines relating to `MidiPitchParser` are commented out, so installing portaudio is not required. To use the whole project, it needs to be [installed](http://files.portaudio.com/docs/v19-doxydocs/tutorial_start.html).| [MIT](http://files.portaudio.com/docs/v19-doxydocs/License.html)
[GLEW](http://glew.sourceforge.net/) | (C) 2002-2007, Milan Ikits <br/> (C) 2002-2007, Marcelo E. Magallon <br/> (C) 2002, Lev Povalahev <br/> Must be installed to compile the project. | [Modified BSD](https://github.com/nigels-com/glew/blob/master/LICENSE.txt), <br/> [MIT](https://github.com/nigels-com/glew/blob/master/LICENSE.txt)
[OpenCV 2](https://opencv.org/) | Must be installed to compile the project. | BSD
[glfw3](https://www.glfw.org/) | (C) 2002-2006 Marcus Geelnard <br/> (C) 2006-2019 Camilla Löwy <br/> Must be installed to compile the project. | [zlib](https://github.com/glfw/glfw/blob/master/LICENSE.md)


## License
Licensed under the MIT Licese. See [LICENSE](LICENSE) for more details.
