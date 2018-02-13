DEBUG_MODE = False
class node():
    position = [None,None]
    travelCost = None
    rootPath = []
    goalCost = None
    traversed = None
    def __init__(self,pos,parent,goal):
        global DEBUG_MODE
        self.position = pos
        self.goalCost = (((pos[0]-goal[0])**2) + ((pos[1]-goal[1])**2))**0.5
        self.traversed = False
        self.solution = None
        if parent is not None:
            self.travelCost = parent.travelCost + 1
            self.rootPath = parent.rootPath[:]
            self.rootPath.append(self.position)
        else:
            self.rootPath = []
            self.travelCost = 0
            self.rootPath.append(self.position)
        if DEBUG_MODE:
            print("DEBUG: Position",self.position)
            print("DEBUG: root",self.rootPath)
            print("DEBUG: goalCost",self.goalCost)
        
     
             
class aStar(object):

    def __init__(self, grid, spawn, goal):
        # Criteria
        self.grid = grid
        self.spawn = spawn
        self.goal = goal

        self.nodeArray = [[None] * len(grid) for _ in range(len(grid))]
        self.nodeArray[spawn[0]][spawn[1]] = node(spawn,None,goal)

    def find_path(self):
        numNodes = 0
        startNode = self.nodeArray[self.spawn[0]][self.spawn[1]]
        priority_queue = [startNode]
        
        while True:
            if len(priority_queue) == 0:
                print('Done')
                return
            current_node = self.topPriority(priority_queue)
            priority_queue.remove(current_node)
            current_node.traversed = True
            self.solution = current_node
            
            for j in [-1,0,1]:
                for i in [-1,0,1]:
                    y = j+ current_node.position[0]
                    x = i+ current_node.position[1]
                    if (x<0) or (x>len(self.grid[0])-1) or (y<0) or (y>len(self.grid)-1) or (i==0 and j==0):
                        continue
                    if self.grid[y][x] == 0:
                        continue
                    if self.nodeArray[y][x] != None:
                        if self.nodeArray[y][x].traversed == True:
                            continue
                        elif self.nodeArray[y][x].travelCost > node([y,x],current_node,self.goal).travelCost:
                            self.nodeArray[y][x] = node([y,x],current_node,self.goal)
                    else:
                        self.nodeArray[y][x] = node([y,x],current_node,self.goal)
                    print("new node")
                    priority_queue.append(self.nodeArray[y][x])
                    # print("root",self.nodeArray[y][x].rootPath)
                    # print("goalCost",self.nodeArray[y][x].goalCost)


            numNodes += 1
            if numNodes > len(self.grid)*len(self.grid):
                exit()
        print('Completed in : '+str(count)+'steps')

    
    
    def topPriority(self, priority_queue):
        print("newQueue")
        lowest_cost = priority_queue[0].travelCost + priority_queue[0].goalCost
        lowest_node = priority_queue[0]
        print(lowest_cost)
        for node in priority_queue[1:]:
            cost = node.travelCost + node.goalCost
            print(cost)
            if cost < lowest_cost:
                
                lowest_node = node
                lowest_cost = cost
        print("selecting",lowest_cost)
        return lowest_node