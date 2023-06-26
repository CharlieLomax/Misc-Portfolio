import java.awt.Point;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.PriorityQueue;
import java.util.Stack;

/**
	Represents an intelligent agent moving through a particular room.	
	The robot only has one sensor - the ability to get the status of any  
	tile in the environment through the command env.getTileStatus(row, col).
	
	Your task is to modify the getAction method below so that is reached the
	TARGET POSITION with a minimal number of steps. There is only one (1)
	target position, which you can locate using env.getTargetRow() and env.getTargetCol()
*/

public class Robot {
	private Environment env;
	private int posRow;
	private int posCol;
	private String searchAlgorithm;
	public int expanded = 0;
	
	private LinkedList<Action> plan;
	private Iterator<Action> currentAction;
	/**
	    Initializes a Robot on a specific tile in the environment. 
	*/
	public Robot (Environment env) { this(env, 0, 0, ""); }
	public Robot (Environment env, int posRow, int posCol, String searchAlgorithm) {
		this.env = env;
		this.posRow = posRow;
		this.posCol = posCol;
		this.searchAlgorithm = searchAlgorithm;
	}
	public int getPosRow() { return posRow; }
	public int getPosCol() { return posCol; }
	public void incPosRow() { posRow++; }
	public void decPosRow() { posRow--; }
	public void incPosCol() { posCol++; }
    public void decPosCol() { posCol--; }
	
    /**
     * Construct search tree before Robot start moving.
     */
    public void plan(){
    	switch(searchAlgorithm){
    	case "DFS":
			/**
			 * Implement your off line DFS search algorithm here.
			 * For clarity, you should implement the algorithm in another function/class and invoke it here.
			 */
    		plan = dfsPlan();
    		currentAction = plan.iterator();
    		
			break;
		case "AStar":
			/**
			 * Implement your off line A* search algorithm here.
			 * For clarity, you should implement the algorithm in another function/class and invoke it here.
			 */
			plan = aStarPlan();
			currentAction = plan.iterator();
			
			break;
		case "RBFS":
			/**
			 * Implement your off line RBFS search algorithm here.
			 * For clarity, you should implement the algorithm in another function/class and invoke it here.
			 */
			plan = rbfsPlan();
			currentAction = plan.iterator();
			
			break;
		case "HillClimbing":
		default:
			break;
    	}
    }
    
	/**
	    Simulate the passage of a single time-step.
	    At each time-step, the Robot decides which direction
	    to move.
	*/
	public Action getAction () {
		// you can get a tile's status with
		//TileStatus status = env.getTileStatus(posRow, posCol);
		
		Action action = Action.DO_NOTHING;
		
		switch(searchAlgorithm){
		case "DFS":
			/**
			 * Follow your constructed search tree in plan() to select actions.
			 */
			return getPlannedAction();
			
		case "AStar":
			/**
			 * Follow your constructed search tree in plan() to select actions.
			 */
			return getPlannedAction();
			
		case "RBFS":
			/**
			 * Follow your constructed search tree in plan() to select actions.
			 */
			return getPlannedAction();
			
		case "HillClimbing":
			/**
			 * Implement your online HillClimbing search algorithm here.
			 * For clarity, you should implement the algorithm in another function/class and invoke it here.
			 */
			return hillClimb();
			
		default:
			break;
    	}
		
	    return action;
	}
	
	private LinkedList<Action> dfsPlan() {
		Stack<Point> stack = new Stack<Point>();
		Stack<Action> actionStack = new Stack<Action>();
		LinkedList<Action> path = new LinkedList<Action>();
		
		stack.push(new Point(posRow, posCol));
		actionStack.push(Action.DO_NOTHING);
		
		while(!stack.empty()) {
			Point curr = stack.pop();
			Action currAction = actionStack.pop();
			
			if(env.validPos(curr.x, curr.y)) {
				path.addLast(currAction);
				expanded++;
				
				if(env.getTileStatus(curr.x, curr.y) == TileStatus.TARGET)
					return path;
				
				stack.push(new Point(curr.x - 1, curr.y));
				actionStack.push(Action.MOVE_UP);
				
				stack.push(new Point(curr.x, curr.y + 1));
				actionStack.push(Action.MOVE_RIGHT);
				
				stack.push(new Point(curr.x + 1, curr.y));
				actionStack.push(Action.MOVE_DOWN);
				
				stack.push(new Point(curr.x, curr.y - 1));
				actionStack.push(Action.MOVE_LEFT);
			}
		}
		
		return null;
	}
	
	private LinkedList<Action> aStarPlan() {
		HashMap<Point, TreeNode<Action>> directions = new HashMap<Point, TreeNode<Action>>();
		HashMap<Point, Double> bestPath = new HashMap<Point, Double>();
		PriorityQueue<FNode> fnodes = new PriorityQueue<FNode>();
		
		Point start = new Point(posRow, posCol);
		directions.put(start, new TreeNode<Action>(Action.DO_NOTHING));
		bestPath.put(start, 0.0);
		fnodes.add((new FNode(env.getTileCost(start.x, start.y) + heuristic(start.x, start.y), start)));
		
		while(!fnodes.isEmpty()) {
			Point curr = fnodes.poll().getNode();
			
			expanded++;
			
			if(curr.equals(new Point(env.getTargetRow(), env.getTargetCol()))) {
				return treePath(directions.get(curr));
			}
			
			Point[] children = {new Point(curr.x + 1, curr.y), new Point(curr.x - 1, curr.y), new Point(curr.x, curr.y + 1), new Point(curr.x, curr.y - 1)};
			Action[] actions = {Action.MOVE_DOWN, Action.MOVE_UP, Action.MOVE_RIGHT, Action.MOVE_LEFT};
			
			double pathCost = bestPath.get(curr);
			
			for(int i = 0; i < 4; i++) {
				double g = pathCost + env.getTileCost(children[i].x, children[i].y);
				
				if(env.validPos(children[i].x, children[i].y) && (bestPath.get(children[i]) == null || bestPath.get(children[i]) > g)) {
					directions.put(children[i], new TreeNode<Action>(actions[i], directions.get(curr)));
					bestPath.put(children[i], g);
					fnodes.add(new FNode(g + heuristic(children[i].x, children[i].y), children[i]));
				}
			}
		}
		
		return null;
	} // Based on pseudocode found at https://en.wikipedia.org/wiki/A*_search_algorithm#Pseudocode
	
	private double heuristic(int row, int col) {
		return Math.sqrt(Math.pow((env.getTargetRow() - row), 2.0) + Math.pow((env.getTargetCol() - col), 2.0));
	}
	
	private LinkedList<Action> treePath(TreeNode<Action> action) {
		LinkedList<Action> path = new LinkedList<Action>();
		TreeNode<Action> node = action;
		while(node != null) {
			path.addFirst(node.getElement());
			node = node.getParent();
		}
		
		return path;
	}
	
	private LinkedList<Action> rbfsPlan() {
		HashMap<Point, Double> fVals = new HashMap<Point, Double>();
		
		fVals.put(new Point(posRow, posCol), 0.0);
		
		return treePath(rbfs(posRow, posCol, null, fVals, 0.0, Double.MAX_VALUE, Action.DO_NOTHING, null).getSolution());
	}
	
	private RBFSSolution rbfs(int row, int col, TreeNode<Action> path, HashMap<Point, Double> fVals, double distance, double fLimit, Action action, Point parent) {
		TreeNode<Action> pathNode = new TreeNode<Action>(action, path);
		
		Point node = new Point(row, col);
		
		expanded++;
		
		if(env.getTargetRow() == row && env.getTargetCol() == col) {
			return new RBFSSolution(distance, pathNode);
		}
		
		Point[] children = {new Point(row + 1, col), new Point(row - 1, col), new Point(row, col + 1), new Point(row, col - 1)};
		Action[] actions = {Action.MOVE_DOWN, Action.MOVE_UP, Action.MOVE_RIGHT, Action.MOVE_LEFT};
		
		for(Point child : children) {
			if(env.validPos(child.x, child.y)) {
				//if(fVals.get(child) == null) {
					fVals.put(child, distance + env.getTileCost(child.x, child.y) + heuristic(child.x, child.y));
				/*}
				else {
					fVals.put(child, Math.max(distance + env.getTileCost(child.x, child.y) + heuristic(child.x, child.y), fVals.get(node)));
				}*/
			}
		}
		
		while(true) {
			double best = Double.MAX_VALUE;
			int bestPos = -1;
			
			double alt = Double.MAX_VALUE;
			for(int i = 0; i < 4; i++) {
				if(!children[i].equals(parent)) {
					Double val = fVals.get(children[i]);
					if(val != null && val < best) {
						alt = best;
						best = val;
						bestPos = i;
					}
					
					else if(val != null && val < alt) {
						alt = val;
					}
				}
			}
			
			if(bestPos >= 0) {
				Point bestNode = children[bestPos];
				
				if(best > fLimit) {
					return new RBFSSolution(best, null);
				}	
				
				RBFSSolution solution = rbfs(bestNode.x, bestNode.y, pathNode, fVals, distance + env.getTileCost(bestNode.x, bestNode.y), Math.min(fLimit, alt), actions[bestPos], node);
				
				fVals.put(bestNode, solution.getF());
				
				if(solution.getSolution() != null) {
					return solution;
				}
			}
		}
	} // Based on pseudocode from the textbook
	
	private Action hillClimb() {
		expanded++;
		
		Point[] neighbors = {new Point(posRow + 1, posCol), new Point(posRow - 1, posCol), new Point(posRow, posCol + 1), new Point(posRow, posCol - 1)};
		Action[] actions = {Action.MOVE_DOWN, Action.MOVE_UP, Action.MOVE_RIGHT, Action.MOVE_LEFT};
		
		double best = Double.MAX_VALUE;
		int bestPos = 0;
		
		for(int i = 0; i < 4; i++) {
			Point curr = neighbors[i];
			double value = env.getTileCost(curr.x, curr.y) + heuristic(curr.x, curr.y);
			if(env.validPos(curr.x, curr.y) && value < best) {
				best = value;
				bestPos = i;
			}
		}
		
		return actions[bestPos];
	}
	
	private Action getPlannedAction() {
		return currentAction.next();
	}
	
	private class TreeNode<E> {
		private E element;
		private TreeNode<E> parent;
		private LinkedList<TreeNode<E>> children;
		
		public TreeNode(E element) {
			this.element = element;
			this.parent = null;
			this.children = new LinkedList<TreeNode<E>>();
		}
		
		public TreeNode(E element, TreeNode<E> parent) {
			this.element = element;
			this.parent = parent;
			this.children = new LinkedList<TreeNode<E>>();
			if(parent != null)
				parent.addChild(this);
		}
		
		public E getElement() {
			return element;
		}
		
		public TreeNode<E> getParent() {
			return parent;
		}

		public void addChild(TreeNode<E> child) {
			children.add(child);
		}
	}
	
	private class RBFSSolution {
		private double f;
		private TreeNode<Action> solution;
		
		public RBFSSolution(double f, TreeNode<Action> solution) {
			this.f = f;
			this.solution = solution;
		}
		
		public double getF() {
			return f;
		}
		public TreeNode<Action> getSolution() {
			return solution;
		}
		
	}
	
	private class FNode implements Comparable<FNode> {
		private double f;
		private Point node;
		
		public FNode(double f, Point node) {
			this.f = f;
			this.node = node;
		}
		
		double getF() {
			return f;
		}
		
		Point getNode() {
			return node;
		}
		
		@Override
		public int compareTo(FNode o) {
			return (int)(f - o.getF());
		}
	}
}