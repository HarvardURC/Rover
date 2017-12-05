import pygame
import time

import sys, termios, tty, os
 
import curses

file = open('controlValues.txt', 'w')


# arrow control in case user doesn't have gamepad
def main(win):
    win.nodelay(True)
    key=""
    win.clear()                
    win.addstr("Hold UP, DOWN, LEFT, RIGHT arrows to control rover (DOWN pauses rover) \n\nDetected key: ")
    while 1:          
        try:                 
            key = win.getkey()         
            win.clear()                
            win.addstr("Hold UP, DOWN, LEFT, RIGHT arrows to control rover (DOWN pauses rover) \n\nDetected key: ")
            key = str(key)
            win.addstr(key) 

            if key == "KEY_LEFT":
                file.seek(0)
                file.write('0')
            elif key == "KEY_DOWN":
                file.seek(0)
                file.write('1')
            elif key == "KEY_RIGHT":
                file.seek(0)
                file.write('2')
            elif key == "KEY_UP":
                file.seek(0)
                file.write('3')

            time.sleep(.1)
            if key == os.linesep:
                break           
        except Exception as e:
            # No input   
            pass    


'''
Gets joystick data and prints it
'''
pygame.init()
#joystick.init()
x = pygame.joystick.get_count()

# if game controller is available, use game controller
if x:
    print "Using Gamepad for control"

    j = pygame.joystick.Joystick(0)
    j.init()
    print ('Initialized Joystick : %s' % j.get_name())

    which_toggle = ["LeftToggle_horizontal", "LeftToggle_vertical", "RightToggle_horizontal", "RightToggle_vertical",]
    which_button = ["button_X", "button_A", "button_B", "button_Y", "button_LB", "button_RB", "button_LT", "button_RT", "button_back", "button_start", "button_toggleleft", "button_toggleright"]
    try:
        while True:
            pygame.event.pump()


            # read input from the two joysticks  
                 
            for i in range(0, j.get_numaxes()):
                # make sure toggle is past a certain cutoff angle
                if abs(j.get_axis(i)) >= 0.07:
                    #print (which_toggle[i], j.get_axis(i), i)
                    
                    file.seek(0)
            
            for i in range(0, j.get_numbuttons()):
                if j.get_button(i) != 0:
                    #print (which_button[i], "button Joystick Num:", i)

                    # write to controlValues.txt which button I'm pressing
                    file.seek(0)
                    file.write(str(i))

            time.sleep(.1)

    except KeyboardInterrupt:
        j.quit()
        file.close()

else :
    print "Switched to keyboard arrow keys for control"
    print "Use UP, DOWN, LEFT, RIGHT arrows to control rover"
    curses.wrapper(main)
    file.close()
    

