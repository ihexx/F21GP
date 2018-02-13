from G2L import aStar

class parameters():
    pWall = 0.3
    startCoordinates = [0,0]
    endCoordinates = [0,0]
    numCells = 10
    grid = []
    squareSize = 20
    init = False
params = parameters()
allow_diagonal_move = True

astar = 0

def setup():
    size(params.numCells * params.squareSize, params.numCells * params.squareSize)
    noLoop()
    new_grid()

def new_grid():
    global params,astar
    
    for _ in range(params.numCells):
        row = [0] * params.numCells
        params.grid.append(row)
    for x in range(params.numCells):
        for y in range(params.numCells):
            if(random(0, 100) < params.pWall*100):
                params.grid[y][x] = 0
            else:
                params.grid[y][x] = 1
    
    params.startCoordinates[0], params.startCoordinates[1] = randomSquare(params.grid) # y/x
    params.endCoordinates[0], params.endCoordinates[1] = randomSquare(params.grid)

    params.grid[params.startCoordinates[0]][params.startCoordinates[1]] = 1
    params.grid[params.endCoordinates[0]][params.endCoordinates[1]] = 1
    
    print(params.startCoordinates)
    print(params.endCoordinates)
    
    astar = aStar(grid=params.grid, spawn=params.startCoordinates, goal=params.endCoordinates)

def randomSquare(grid):
    global params
    x = int(random(0, len(grid[0])))
    y = int(random(0, len(grid)))
    return y,x

def draw():
    global params
    render(params.grid)
    if params.init:
        draw_path(astar)
    drawSquare(params.startCoordinates,color(255, 125, 0))
    drawSquare(params.endCoordinates,color(0, 125, 255))
    

def keyPressed():
    global params,astar
    if key == 'c':
        astar.find_path()
        params.init = True
        redraw()
        

def render(grid):
    global params
    for x in range(len(grid)):
        for y in range(len(grid)):
            if (grid[y][x]):
                fill(color(255,255,255))
            else:
                fill(color(0,0,0))
            rect((x) * params.squareSize, y * params.squareSize, params.squareSize, params.squareSize)
            
def drawSquare(coordinates,colors):
    fill(colors)
    rect(coordinates[1] * params.squareSize,
         coordinates[0] * params.squareSize,
         params.squareSize,params.squareSize)

def draw_path(astar):
    global params
    fill(color(255,0,0,125))
    for coords in astar.solution.rootPath:
        rect(coords[1] * params.squareSize,
            coords[0] * params.squareSize,
            params.squareSize,
            params.squareSize)
 

    

    