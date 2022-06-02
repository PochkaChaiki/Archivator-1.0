from tkinter import *
from tkinter import filedialog as fd
import subprocess as sb

window = Tk()
window.geometry("900x600")
window.title("Archivator 1.0.1")

str_list = ['./test.exe ', '-c']
rvar = BooleanVar()
rvar.set(0)
tvar = BooleanVar()# TRUE - hfm, FALSE - lzw
tvar.set(0)
 
def select_files():
    global filenames
    filetypes = (('All files', '*.*'), ('text files', '*.txt'), ('zipper files', '*.zipper'))
    filenames = fd.askopenfilenames(title = 'Choose files to compress', initialdir = '/', filetypes = filetypes)
    flabel.configure(text = str(filenames))

def select_path():
    global path
    path = fd.askdirectory(initialdir = '/')  
    pathlabel.configure(text = path)

def runcmd():
    str_list = ['./test.exe ', '-c']
    if (rvar.get() == TRUE):
        str_list.append('-pack')
    else:
        str_list.append('-unpack')
    str_list.append('-path')
    str_list.append(path.replace("/", "\\"))
    str_list.append('-files')
    for i in filenames:
        str_list.append(i.replace("/", "\\"))
    if (tvar.get() == TRUE) and (rvar.get() == TRUE):
        str_list.append('-alg')
        str_list.append('hfm')
    elif (tvar.get() == FALSE) and (rvar.get() == TRUE):
        str_list.append('-alg')
        str_list.append('lzw')
    sb.run(str_list)

main_fr = Frame(window)
package = Frame(main_fr)
algfr = Frame(main_fr)

runbtn = Button(main_fr, text = "Run Program", command = runcmd)
btnfiles = Button(main_fr, text="Choose your files", command = select_files)
btnpath = Button(main_fr, text="Choose the path", command = select_path)

pack_radiobtn = Radiobutton(package, text = "pack", variable = rvar, value = TRUE)
unpack_radiobtn = Radiobutton(package, text = "unpack", variable = rvar,value = FALSE)
hfm_radiobtn = Radiobutton(algfr, text = "Hoffman algorithm", variable = tvar, value = TRUE)
lzw_radiobtn = Radiobutton(algfr, text = "LZW algorithm", variable = tvar, value = FALSE)

flabel = Label(main_fr, text = "Choose files")
pathlabel = Label(main_fr, text = "Choose path where to pack/unpack")

pack_radiobtn.pack()
unpack_radiobtn.pack()
hfm_radiobtn.pack()
lzw_radiobtn.pack()

package.grid(column = 1, row = 5, padx = 10, pady = 5)
algfr.grid(column = 2, row = 5, padx = 10, pady = 5)
btnfiles.grid(column = 1, row = 1, padx = 10, pady = 3)
flabel.grid(column = 2, row = 1, padx = 10, pady = 3)
btnpath.grid(column = 1, row = 2, padx = 10, pady = 3)
pathlabel.grid(column = 2, row = 2, padx = 10, pady = 3)
runbtn.grid(column = 1, row = 3, columnspan = 2, padx = 20, pady = 10)

main_fr.pack()

window.mainloop()
