from PIL import Image

background = Image.open("obstacle_astar.png")
overlay = Image.open("obstacles.png")

background = background.convert("RGBA")
overlay = overlay.convert("RGBA")

new_img = Image.blend(background, overlay, 0.5)
new_img.save("blended.png","PNG")