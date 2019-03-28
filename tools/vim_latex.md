## Install vim-plug
```
$ mkdir ~/.vim/autoload
$ cd ~/.vim/autoload
$ wget https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
$ vim ~/.vimrc
```
Then set the plugs you want to install, [example](https://github.com/junegunn/vim-plug)
```
call plug#begin('~/.vim/plugged')

" On-demand loading
Plug 'scrooloose/nerdtree', { 'on':  'NERDTreeToggle' }

call plug#end()
```
Finally, enter vim and install plugs:
`:PlugInstall`

## Install texlive
- texlive-base - 136 MB
- texlive-latex-recommended - 177 MB
- texlive - 240 MB
- texlive-latex-extra - 404 MB
- texlive-full - 4714 MB
```
$ sudo apt install texlive-latex-extra
```
Compile latex file:
```
$ $ pdflatex hello-world.tex
```
## Install vimtex dependencies
Compiler: latexmk is a package
```
# sudo apt-get install -y latexmk
```
PDF reader:
``
$ sudo apt-get install zathura
$ sudo apt-get install xdotool
```
## vimrc setting
```
let g:vimtex_view_method='zathura'
let g:vimtex_quickfix_mode=0
set conceallevel=1
let g:tex_conceal='abdmg'
```
[A tutorial](https://castel.dev/post/lecture-notes-1/)
