import curses
import time

def main(win):
    
    win.nodelay(True)
    key=""
    win.clear()                
    win.addstr("Detected key:")
    while 1:          
        try:                 
           key = win.getkey()
           print key, key  
                
           win.clear()                
           win.addstr("Detected key:")
           win.addstr(str(key)) 
           
           if key == os.linesep:
              break           
        except Exception as e:
           # No input   
           pass        

curses.wrapper(main)