package main;


import java.util.LinkedList;

import main.Line.Direction;
import main.MovementTree.TreeNode;

public class AIPlayer extends Player {

	private MovementTree tree = new MovementTree();
	
	/** 
	 * The depth the search tree is extended to.
	 * Currently very low.
	 */
	private int searchDepth = 3;
	
	public AIPlayer(char token) {
		super(token);
		
		//A placeholder node in the root position, not sure the best way to handle this
		tree.addNode(null, null, new LinkedList<Line>(), new LinkedList<Line>(), false);
	}

	/**
	 * Returns the next move the AI will make
	 */
	@Override
	public Move getMove(char[][] playSpace, int playSize, Move lastMove) {
		System.out.println("Performing AI move:");
		
		makeOpponentMove(tree, lastMove);
		
		//Calls helper function
		makeMoveBranch(playSpace, playSize, tree.getRoot(), 0, searchDepth);
		
		//Evaluates the value of each node to determine the best move
		tree.moveScoreUp();
		
		//Finds the best move possible, picking the first one if there is a tie.  Returns the node containing the move 
		TreeNode newRoot = tree.nextMove();
		//Makes that best move node the root
		tree.newRoot(newRoot);
		//Removes parent from the root
		newRoot.setParent(null);
		//Returns the move of the best move node 
		Move nextMove = tree.getRoot().getMove();
		
		//Adds any lines that will be created by the move
		lineHelper(playSpace, nextMove);
		
		return nextMove;
	}
	
	void makeOpponentMove(MovementTree tree, Move move) {
		TreeNode root = tree.getRoot();
		if(root != null && !root.isMinNode() && root.getChildren() != null) {
			for(TreeNode node : root.getChildren()) {
				if(node.getMove().equals(move)) {
					tree.newRoot(node);
				}
			}
		}
	}
	
	/**
	 * Helper method, recursively builds the tree 
	 * @param playSpace The grid the game is played on 
	 * @param playSize The n by n size of the grid 
	 * @param last The node this iteration is building on 
	 * @param currDepth The current depth of the tree
	 * @param maxDepth The depth the tree is going to
	 */
	private void makeMoveBranch(char[][] playSpace, int playSize, TreeNode last, int currDepth, int maxDepth) {
		//Exits if the depth is to the maximum value 
		if(currDepth == maxDepth) {
			return;
		}
		
		if(last.getChildren().size() == 0) {
			//Iterates through the whole grid
			//Adding empty spaces to the tree
			for(int i = 0; i < playSize; i++) {
				for(int j = 0; j < playSize; j++) {
					if(playSpace[i][j] == 0) {
						Move move = new Move(i, j);
						TreeNode curr = tree.addNode(move, last, childLine(playSpace, move, last.getPlayerLines()), childLine(playSpace, move, last.getOpponentLines()), !last.isMinNode());
						if(!last.isMinNode()) {
							playSpace[i][j] = getPlayerToken();
						}
						else {
							playSpace[i][j] = 'N';
						}
						//Recursive call, moves the current depth down and the last node down to the current as well 
						makeMoveBranch(playSpace, playSize, curr, currDepth + 1, maxDepth);
						playSpace[i][j] = 0;
					}
				}
			}
		}
		
		else {
			for(TreeNode child : last.getChildren()) {
				makeMoveBranch(playSpace, playSize, child, currDepth + 1, maxDepth);
			}
		}
	}
	
	public LinkedList<Line> childLine(char[][] playSpace, Move nextMove, LinkedList<Line> lines) {
		LinkedList<Line> childLines = new LinkedList<Line>();
		
		// Make a copy of the lines
		for(Line line : lines) {
			childLines.add(new Line(line));
		}
		
		//Adds all single lines this can make
		Move aloneMoveV1 = new Move(nextMove.row, nextMove.col);
		Move aloneMoveV2 = new Move(nextMove.row, nextMove.col);
		Line aloneLineV = new Line(Direction.VERTICAL, aloneMoveV1, aloneMoveV2);
		childLines.add(aloneLineV);
		
		Move aloneMoveH1 = new Move(nextMove.row, nextMove.col);
		Move aloneMoveH2 = new Move(nextMove.row, nextMove.col);
		Line aloneLineH = new Line(Direction.HORIZONTAL, aloneMoveH1, aloneMoveH2);
		childLines.add(aloneLineH);
		
		Move aloneMoveL1 = new Move(nextMove.row, nextMove.col);
		Move aloneMoveL2 = new Move(nextMove.row, nextMove.col);
		Line aloneLineL = new Line(Direction.DOWN_LEFT, aloneMoveL1, aloneMoveL2);
		childLines.add(aloneLineL);
		
		Move aloneMoveR1 = new Move(nextMove.row, nextMove.col);
		Move aloneMoveR2 = new Move(nextMove.row, nextMove.col);
		Line aloneLineR = new Line(Direction.DOWN_RIGHT, aloneMoveR1, aloneMoveR2);
		childLines.add(aloneLineR);

		
		//Combines with any already existing lines
		for(int i = 0; i < childLines.size() - 4; i++) {
			//Makes a new line that can be edited
			Line tempLine = new Line(childLines.get(i));
			//Advances both ends of the temporary line
			tempLine.updateEndPos1();
			tempLine.updateEndPos2();
			boolean sameDir = childLines.get(i).getDir() == tempLine.getDir();
			//Checks to see if end1 needs to be added
			if(nextMove.equals(tempLine.getEndPos1()) && sameDir) {
				//System.out.println("Expand1");
				//Extends the line to include the new point
				childLines.get(i).updateEndPos1();
			}
			else if(nextMove.equals(tempLine.getEndPos2()) && sameDir) {
				//System.out.println("Expand2");
				childLines.get(i).updateEndPos2();
			}
		}

		return childLines;
	}
}
