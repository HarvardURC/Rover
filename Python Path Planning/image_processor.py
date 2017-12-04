from PIL import Image
import numpy as np
# background = Image.open("obstacle_astar.png")
# overlay = Image.open("obstacles.png")

# background = background.convert("RGBA")
# overlay = overlay.convert("RGBA")

# new_img = Image.blend(background, overlay, 0.5)
# new_img.save("blended.png","PNG")

# change certain color 
im = Image.open('obstacle_astar.png')
data = np.array(im)

print data

r1, g1, b1 = 255, 255, 255 # Original value
r2, g2, b2 = 125, 0, 0 # Value that we want to replace it with

red, green, blue = data[:,:,0], data[:,:,1], data[:,:,2]
# red, green, blue = data[:,0], data[:,1], data[:,2]
mask = (red == r1) & (green == g1) & (blue == b1)
data[:,:,:3][mask] = [r2, g2, b2]
# data[:,:3][mask] = [r2, g2, b2]

im = Image.fromarray(data)
im.save('obstacle_astar_marked.png')