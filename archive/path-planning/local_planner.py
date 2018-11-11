import util, copy, math, time
from PIL import Image
import numpy as np
import heightmap_planner as plan
import visualizer as vis

delta = {'N': (0, 1),
    'S': (0, -1),
    'E': (1, 0),
    'W': (-1, 0),
    'NW': (-1, 1),
    'SW': (-1, -1),
    'NE': (1, 1),
    'SE': (1, -1),
    'STOP': (0, 0)}

# FOR ROVER USE (3D Framework)
# # constant inputs
# # sensor_data = [[tilted row], [horizontal row]]
# sensor_data = [[None, None, 20, None], [5, 9, 10, 3]]
# # gyro_data = [pitch, yaw]
# gyro_data = [0, 0]
# # compass = [0 to 359 where 0 is North]
# compass = [0]

# max_height
# slope
# sensor_angle
# gyro_data
# compass

# turn by x degrees or move forward 

# 2D Version

size = 129

# obstacle creation
obstacles = {}

def makeline(point1, point2):
    x1, y1 = point1
    x2, y2 = point2
    for x in range(x1, x2+1):
        for y in range(y1, y2+1):
            obstacles[(plan.adj_x() * x, plan.adj_y() * -y)] = 125

# U starting obstacle
makeline((-4, -4), (4, -4))
makeline((-4, -4), (-4, 4))
makeline((4, -4), (4, 4))
makeline((-4, 4), (-2, 4))
makeline((2, 4), (4, 4))

# up-side down T
makeline((6, -10), (20, -10))
makeline((13, -10), (13, 0))

# horizontal lines
makeline((1, -15), (10, -15))
makeline((18, -15), (25, -15))
makeline((-10, -20), (1, -20))
makeline((-10, -20), (1, -20))
makeline((25, -50), (50, -50))
makeline((-10, -56), (20, -56))
makeline((18, -62), (30, -62))

# vertical lines
makeline((6, -40), (6, -30))
makeline((18, -40), (18, -30))
makeline((19, -40), (19, -30))
makeline((20, -40), (20, -30))
makeline((21, -40), (21, -30))
makeline((22, -40), (22, -30))
makeline((23, -40), (23, -30))
makeline((24, -40), (24, -30))
makeline((25, -40), (25, -30))

data = vis.map_to_array(obstacles, size)
# with open("obstacles.png","wb") as f:
#     f.write(vis.makeGrayPNG(data))

# SLAM-like algorithm
# input: state
# output: list of obstacles within diamond (max 2 straight and 1 diagonal)
def lidar(state):
    x, y = state
    result = []
    for dx in range(-1, 2):
        for dy in range(-1, 2):
            new_state = (x + dx, y + dy)
            if new_state in obstacles:
                result.append(new_state)
    for dx, dy in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
        new_state = (x + dx, y + dy)
        if new_state not in result:
            if dx == 1 and (x + 2, y) in obstacles: result.append((x + 2, y))
            if dx == -1 and (x - 2, y) in obstacles: result.append((x - 2, y))
            if dy == 1 and (x, y + 2) in obstacles: result.append((x, y + 2))
            if dy == -1 and (x, y - 2) in obstacles: result.append((x, y - 2))
    return result

# print lidar((-2, 0))
# print obstacles.keys()
results = []

result = []
loop = 0

### Local Planning with New Objects Detected ###
# loop to run algorithm 5 times for timing averages
while loop < 5:
    environment = {}
    cur_state = plan.getStartState()
    times = []
    counter = 0
    coordinates = [cur_state]

    # preload an image of the obstacles on the heightmap
    png = plan.data()
    png = vis.mod_array(obstacles, png, 125)

    start_time = time.time()
    while not plan.goal(cur_state):
        for ob in lidar(cur_state):
            environment[ob] = 255
        actions = plan.aStarSearch(environment.keys(), cur_state)
        for action in actions:
            x, y = cur_state[0] + 64, 64 + cur_state[1]
            png[y][x] = 254
            x, y = cur_state
            dx, dy = delta[action]
            counter += 1

            # to generate run times and visuals at different time steps
            if counter == 1:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("obstacle_astar_1.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 15:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("obstacle_astar_15.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 30:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("obstacle_astar_30.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 45:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("obstacle_astar_45.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 60:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("obstacle_astar_60.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 75:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("obstacle_astar_75.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 90:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("obstacle_astar_90.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 105:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("obstacle_astar_105.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 120:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("obstacle_astar_120.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            cur_state = (x + dx, y + dy)
            coordinates.append(cur_state)
            if not all([x in environment for x in lidar(cur_state)]):
                break
    result.append(times)
    loop += 1

results.append(result)

png = vis.mod_array(environment, png, 255)

with open("lidar_trig_topleft.png","wb") as f:
    f.write(vis.makeGrayPNG(png))
with open("lidar_trig_topleft.txt","wb") as f:
    f.write(str(coordinates))

result = []
loop = 0

### Local Planning with Collision Detected ##
# loop to run algorithm 5 times for timing averages
while loop < 5:
    environment = {}
    cur_state = plan.getStartState()
    times = []
    counter = 0
    coordinates = [cur_state]

    # preload an image of the obstacles on the heightmap
    png = plan.data()
    png = vis.mod_array(obstacles, png, 125)

    start_time = time.time()
    while not plan.goal(cur_state):
        actions = plan.aStarSearch(environment.keys(), cur_state)
        for action in actions:
            x, y = cur_state[0] + 64, 64 + cur_state[1]
            png[y][x] = 254
            x, y = cur_state
            for ob in lidar(cur_state):
                environment[ob] = 255
            dx, dy = delta[action]
            counter += 1

            # to generate run times and visuals at different time steps
            if counter == 1:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("collision_astar_1.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 15:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("collision_astar_15.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 30:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("collision_astar_30.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 45:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("collision_astar_45.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 60:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("collision_astar_60.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 75:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("collision_astar_75.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 90:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("collision_astar_90.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 105:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("collision_astar_105.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if counter == 120:
                times.append((time.time() - start_time))
                png_tmp = vis.mod_array(environment, png, 255)
                with open("collision_astar_120.png","wb") as f:
                    f.write(vis.makeGrayPNG(png_tmp))
            if (x + dx, y + dy) in environment:
                counter -= 1
                break
            cur_state = (x + dx, y + dy)
            coordinates.append(cur_state)
    result.append(times)
    loop += 1

results.append(result)

png = vis.mod_array(environment, png, 255)

with open("collision_trig_topright.png","wb") as f:
    f.write(vis.makeGrayPNG(png))
with open("collision_trig_topright.txt","wb") as f:
    f.write(str(coordinates))

# Output run time results
# First half of list is obstacle triggered with 5 trials
# Second half of list is collision triggered with 5 trials
with open("run_time_bottom_right.txt","wb") as f:
    f.write(str(results))