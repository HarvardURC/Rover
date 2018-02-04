import pygame
import time


'''
Gets joystick data and prints it
'''
pygame.init()
#joystick.init()
x = pygame.joystick.get_count()
print( x )
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
                print (which_toggle[i], j.get_axis(i), i)
        
        for i in range(0, j.get_numbuttons()):
            if j.get_button(i) != 0:
                print (which_button[i], "button Joystick Num:", i)

        events = pygame.event.get()
        for event in events:
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT:
                    print("left")
                elif event.key == pygame.K_RIGHT:
                    print("right")
        
        time.sleep(.1)

except KeyboardInterrupt:
    j.quit()
