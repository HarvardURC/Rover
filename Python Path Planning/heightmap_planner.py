import util
import visualizer as vis
import copy
import math

# Top left:
i = 1
# Top right:
# i = 2
# Bottom left:
# i = 3
# Bottom right:
# i = 4

def adj_x():
    if i == 1 or i == 3:
        return -1
    return 1

def adj_y():
    if i == 1 or i == 2:
        return -1
    return 1

# scaling factor for distance in x, y, z
x_scale = 50
y_scale = 50
z_scale = 20
goal_state = (adj_x() * 64, adj_y() * 64)

# Dictionary of coordinate changes based on direction
delta = {'N': (0, 1),
    'S': (0, -1),
    'E': (1, 0),
    'W': (-1, 0),
    'NW': (-1, 1),
    'SW': (-1, -1),
    'NE': (1, 1),
    'SE': (1, -1),
    'STOP': (0, 0)}

# load ieghtmap file as list of [x, y, height]
inputfile = open('heightmap.txt')
lines = inputfile.read()[2:-2].split('), (')

height_map = dict()

for line in lines:
	val = line.split(', ')
	x = -int(float(val[0])-0.5)
	y = int(float(val[1])+0.5)
	h = int(val[2])
	height_map[(-x, -y)] = h

# visualization of landscape
size = int(math.sqrt(len(lines)))
def data(): return vis.map_to_array(height_map, size)
with open("landscape.png","wb") as f:
	f.write(vis.makeGrayPNG(data()))

def getStartState():
	return (0, 0) # middle of board

def goal(state):
	return (state == goal_state)

def distance(state1, state2):
	x1, y1 = state1
	x2, y2 = state2
	h1, h2 = height_map[state1], height_map[state2]
	return math.sqrt(
		(x_scale*(x1-x2))**2 + (y_scale*(y1-y2))**2 + (z_scale*(h1-h2))**2)

# gives neighboring states as a list given rover's state
def getSuccessors(state):
	result = []
	actions = ['N', 'S', 'E', 'W', 'NW', 'SW', 'NE', 'SE']
	x, y = state
	for action in actions:
		dx, dy = delta[action][0], delta[action][1]
		new_state = (x + dx, y + dy)
		if new_state in height_map:
			result.append((new_state, action, distance(new_state, state)))
	return result

def depthFirstSearch():
    """
    Search the deepest nodes in the search tree first.
    """
    state = getStartState()
    nodes = util.Stack()
    path = util.Stack()
    nodes.push((state, path))
    states_visited = set()

    while True:
        if nodes.isEmpty(): # no nodes to expand
            return util.raiseNotDefined()
        while state in states_visited: # expand nodes that aren't visited
            (state, path) = nodes.pop()
        if goal(state): # check if node is goal
            return path.list
        children = getSuccessors(state) # get children nodes
        # create new path for each child
        for child in children:
            new_path = copy.deepcopy(path)
            new_path.push(child[1])
            nodes.push((child[0], new_path))
        states_visited.add(state) # add expanded node as visited

# print depthFirstSearch()

def breadthFirstSearch():
    """Search the shallowest nodes in the search tree first."""
    # similar to above except fringe is stored as Queue instead of Stack

    state = getStartState()
    nodes = util.Queue()
    path = util.Stack()
    nodes.push((state, path))
    states_visited = set()

    while True:
        if nodes.isEmpty():
            return util.raiseNotDefined()
        while state in states_visited:
            (state, path) = nodes.pop()
        if goal(state):
            return path.list
        children = getSuccessors(state)
        for child in children:
            new_path = copy.deepcopy(path)
            new_path.push(child[1])
            nodes.push((child[0], new_path))
        states_visited.add(state)

# print breadthFirstSearch()

def heuristic(node):
	return distance(node[0], goal_state)

def uniformCostSearch():
    """Search the node of least total cost first."""
    # give the cost of getting to the node
    # def priority(node):
    #     return len(node[1].list)

    # smiliar to above except fringe is stored as PriorityQueue
    state = getStartState()
    nodes = util.PriorityQueueWithFunction(heuristic)
    path = util.Stack()
    nodes.push((state, path))
    states_visited = set()

    while True:
        if nodes.isEmpty():
            return util.raiseNotDefined()
        while state in states_visited:
            (state, path) = nodes.pop()
        if goal(state):
            return path.list
        children = getSuccessors(state)
        for child in children:
            new_path = copy.deepcopy(path)
            new_path.push(child[1])
            nodes.push((child[0], new_path))
        states_visited.add(state)

# print uniformCostSearch()

def aStarSearch(obstacles = [], start_state = getStartState()):
    """Search the node that has the lowest combined cost and heuristic first."""
    # give the forward and back cost as the heuristic
    def priority(node):
        h = heuristic(node)
        def f1(x): return x[1]
        g = sum(map(f1, node[1].list))
        return h + g

    # similar to above
    state = start_state
    nodes = util.PriorityQueueWithFunction(priority)
    path = util.Stack()
    nodes.push((state, path))
    states_visited = set(obstacles)

    while True:
        if nodes.isEmpty():
            return util.raiseNotDefined()
        while state in states_visited:
            (state, path) = nodes.pop()
        if goal(state):
            def f0(x): return x[0]
            return map(f0, path.list)
        children = getSuccessors(state)
        for child in children:
            new_path = copy.deepcopy(path)
            new_path.push((child[1], child[2]))
            nodes.push((child[0], new_path))
        states_visited.add(state)

# visualize path on landscape
# data = vis.map_to_array(height_map, size)
# data_uc = data()
data_astar = data()
cur_state = getStartState()
# actions = uniformCostSearch()
actions = aStarSearch()
coordinates = [cur_state]
while actions:
    x, y = 64 + cur_state[0], 64 + cur_state[1]
    data_astar[y][x] = 255
    # data_uc[y][x] = 255
    x, y = cur_state
    dx, dy = delta[actions.pop(0)]
    cur_state = (x + dx, y + dy)
    coordinates.append((cur_state[1], cur_state[0]))

# with open("landscape_uc.png","wb") as f:
#     f.write(vis.makeGrayPNG(data_uc))
# with open("coordinates_uc.txt","wb") as f:
#     f.write(str(coordinates))
with open("topleft_astar.png","wb") as f:
    f.write(vis.makeGrayPNG(data_astar))
with open("topleft_astar.txt","wb") as f:
    f.write(str(coordinates))
