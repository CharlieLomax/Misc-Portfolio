package cwlomax_Assign2;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Scanner;

public class Futoshiki {
	
	public static void main(String[]args) {
		if(args.length != 2) {
			throw new IllegalArgumentException("Expected 2 inline arguments");
		}
		
		String algorithm = args[0];
		String inputFile = args[1];
		
		Scanner scan;
		try {
			scan = new Scanner(new File(inputFile));
		} catch (FileNotFoundException e) {
			throw new IllegalArgumentException("Invalid file");
		}
		
		int n = scan.nextInt();
		
		scan.nextLine();
		
		String[] indices = scan.nextLine().trim().split(" ");
		String[] values = scan.nextLine().trim().split(" ");
		
		// Map of values already assigned, with their indices as the key
		//HashMap<Integer, Integer> assignedVals = new HashMap<Integer, Integer>();
		int[][] grid = new int[n][n];
		
		for(int i = 0; i < indices.length; i++) {
			//assignedVals.put(Integer.parseInt(indices[i]), Integer.parseInt(values[i]));
			int curr = Integer.parseInt(indices[i]);
			grid[curr / n][curr % n] = Integer.parseInt(values[i]);
		}
		
		String[] greaterIndices = scan.nextLine().trim().split(" ");
		String[] lesserIndices = scan.nextLine().trim().split(" ");
		
		scan.close();
		
		// Map of indices in which the key index is greater than the value index
		HashMap<Integer, Integer> greaterThan = new HashMap<Integer, Integer>();
		// Map of indices in which the key index is less than the value index
		HashMap<Integer, Integer> lessThan = new HashMap<Integer, Integer>();
		
		for(int i = 0; i < greaterIndices.length; i++) {
			int a = Integer.parseInt(greaterIndices[i]);
			int b = Integer.parseInt(lesserIndices[i]);
			
			greaterThan.put(a, b);
			lessThan.put(b, a);
		}
		
		LinkedList<Integer> list = new LinkedList<Integer>();
		for(int i = 1; i <= n; i++) {
			list.add(i);
		}
		
		HashMap<Integer, LinkedList<Integer>> domains = new HashMap<Integer, LinkedList<Integer>>();
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				domains.put(i*n + j, (LinkedList<Integer>) list.clone());
			}
		}
		
		LinkedList<int[][]> solutions = new LinkedList<int[][]>();
		backtracking(grid, domains, greaterThan, lessThan, n, solutions, algorithm);
		
		for(Iterator<int[][]> it = solutions.iterator(); it.hasNext();) {
			int[][] solution = it.next();
			for(int i = 0; i < n; i++) {
				for(int j = 0; j < n; j++) {
					System.out.print(solution[i][j] + " ");
				}
				System.out.println();
				if(it.hasNext()) {
					System.out.println("###############");
				}
			}
		}
	}
	
	private static boolean backtracking(int[][] grid, HashMap<Integer, LinkedList<Integer>> domains, HashMap<Integer, Integer> greaterThan, HashMap<Integer, Integer> lessThan, int n, LinkedList<int[][]> solutions, String algorithm) {
		int col = 0;
		int row = 0;
		
		int val = grid[row][col];
		
		// Search array for first unassigned value
		while(val != 0) {
			col++;
			
			if(col == n) {
				col = 0;
				row++;
			}
			
			if(row == n) {
				solutions.add(grid);
				return true;
			}
			
			val = grid[row][col];
		}
		
		LinkedList<Integer> currentDomain = domains.get(row * n + col);
		for(int i = 0; i < currentDomain.size(); i++) {
			int curr = currentDomain.get(i);
			// Make a copy of the solution array
			int[][] tempSolution = copyGrid(grid);
			HashMap<Integer, LinkedList<Integer>> tempDomains = copyDomain(domains);
			
			if(checkValue(curr, n, row, col, tempSolution, greaterThan, lessThan)) {
				tempSolution[row][col] = curr;
				
				if(inference(n, row, col, tempSolution, tempDomains, greaterThan, lessThan, algorithm)) {
					if(backtracking(tempSolution, tempDomains, greaterThan, lessThan, n, solutions, algorithm)) {
						return true;
					}
				}
			}
		}
		
		return false;
	}
	
	private static boolean checkValue(int val, int n, int row, int col, int[][] grid, HashMap<Integer, Integer> greaterThan, HashMap<Integer, Integer> lessThan) {
		// Check if the value is already in the current row/column
		for(int j = 0; j < n; j++) {
			if(grid[j][col] == val || grid[row][j] == val) {
				return false;
			}
		}
		
		// Check if the value violates an inequality requirement
		int flatIndex = n * row + col;
		Integer smaller = greaterThan.get(flatIndex);
		Integer greater = lessThan.get(flatIndex);
		int smallerVal = 0;
		int greaterVal = 0;
		
		if(smaller != null) {
			smallerVal = grid[smaller / n][smaller % n];
		}
		if(greater != null) {
			greaterVal = grid[greater / n][greater % n];
		}
		
		if((smallerVal != 0 && smallerVal > val) || (greaterVal != 0 && greaterVal < val)) {
			return false;
		}
		
		return true;
	}
	
	private static boolean inference(int n, int row, int col, int[][] grid, HashMap<Integer, LinkedList<Integer>> domains, HashMap<Integer, Integer> greaterThan, HashMap<Integer, Integer> lessThan, String algorithm) {
		switch(algorithm) {
		case "FC":
			for(int i = 0; i < n; i++) {
				LinkedList<Integer> colDomain = domains.get(i * n + col);
				for(int j = 0; j < colDomain.size(); j++) {
					if(grid[i][col] == 0) {
						if(!checkValue(colDomain.get(j), n, i, col, grid, greaterThan, lessThan)) {
							colDomain.remove(j);
							j--;
							
							// If this variable has no legal values, the search has failed
							if(colDomain.size() == 0) {
								return false;
							}
						}
						else if(colDomain.size() == 1) {
							grid[i][col] = colDomain.getFirst();
						}
					}
				}
				
				LinkedList<Integer> rowDomain = domains.get(row * n + i);
				for(int j = 0; j < rowDomain.size(); j++) {
					if(grid[row][i] == 0) {
						if(!checkValue(rowDomain.get(j), n, row, i, grid, greaterThan, lessThan)) {
							rowDomain.remove(j);
							j--;
							
							// If this variable has no legal values, the search has failed
							if(rowDomain.size() == 0) {
								return false;
							}
						}
						else if(rowDomain.size() == 1) {
							grid[row][i] = rowDomain.getFirst();
						}
					}
				}
			}
			
			break;
			
		case "MAC":
			LinkedList<Edge> queue = new LinkedList<Edge>();
			for(int i = 0; i < n; i++) {
				for(int j = 0; j < n; j++) {
					addNeighbors(queue, n, i, j, greaterThan, lessThan);
				}
			}
			
			while(!queue.isEmpty()) {
				Edge edge = queue.pollFirst();
				
				boolean removed = false;
				
				LinkedList<Integer> firstDomain = domains.get(n * edge.getFirstRow() + edge.getFirstCol());
				LinkedList<Integer> secondDomain = domains.get(n * edge.getSecondRow() + edge.getSecondCol());
				
				for(int i = 0; i < firstDomain.size(); i++) {
					boolean valid = false;
					for(int j = 0; j < secondDomain.size(); j++) {
						if((edge.isGreater() && firstDomain.get(i) > secondDomain.get(j)) || (edge.isLess() && firstDomain.get(i) < secondDomain.get(j)) || (firstDomain.get(i) != secondDomain.get(j))) {
							valid = true;
						}
					}
					
					if(!valid) {
						firstDomain.remove(i);
						removed = true;
					}
				}
				
				if(removed) {
					addNeighbors(queue, n , row, col, greaterThan, lessThan);
				}
			}
			
			break;
		}
		
		return true;
	}
	
	private static int[][] copyGrid(int[][] grid) {
		int[][] copy = new int[grid.length][grid[0].length];
		for(int i = 0; i < grid[0].length; i++) {
			System.arraycopy(grid[i], 0, copy[i], 0, grid[0].length);
		}
		return copy;
	}
	
	private static HashMap<Integer, LinkedList<Integer>> copyDomain(HashMap<Integer, LinkedList<Integer>> domains) {
		HashMap<Integer, LinkedList<Integer>> copy = new HashMap<Integer, LinkedList<Integer>>();
		for(HashMap.Entry<Integer, LinkedList<Integer>> entry : domains.entrySet()) {
			copy.put(entry.getKey(), (LinkedList<Integer>) entry.getValue().clone());
		}
		
		return copy;
	}
	
	private static void addNeighbors(LinkedList<Edge> queue, int n, int row, int col, HashMap<Integer, Integer> greaterThan, HashMap<Integer, Integer> lessThan) {
		for(int i = 0; i < n; i++) {
			if(i != row) {
				queue.addLast(new Edge(i, col, row, col));
			}
			if(i != col) {
				queue.addLast(new Edge(row, i, row, col));
			}
		}
		
		int flatIndex = n * row + col;
		Integer smaller = greaterThan.get(flatIndex);
		Integer greater = lessThan.get(flatIndex);
		if(smaller != null) {
			queue.addLast(new Edge(smaller / n, smaller % n, row, col, false, true));
		}
		if(greater != null) {
			queue.addLast(new Edge(greater / n, greater % n, row, col, true, false));
		}
	}
	
	private static class Edge {
		int firstRow;
		int firstCol;
		int secondRow;
		int secondCol;
		boolean greater;
		boolean less;
		public Edge(int firstRow, int firstCol, int secondRow, int secondCol) {
			this.firstRow = firstRow;
			this.firstCol = firstCol;
			this.secondRow = secondRow;
			this.secondCol = secondCol;
			greater = false;
			less = false;
		}
		public Edge(int firstRow, int firstCol, int secondRow, int secondCol, boolean greater, boolean less) {
			this.firstRow = firstRow;
			this.firstCol = firstCol;
			this.secondRow = secondRow;
			this.secondCol = secondCol;
			this.greater = greater;
			this.less = less;
		}
		public int getFirstRow() {
			return firstRow;
		}
		public int getFirstCol() {
			return firstCol;
		}
		public int getSecondRow() {
			return secondRow;
		}
		public int getSecondCol() {
			return secondCol;
		}
		public boolean isGreater() {
			return greater;
		}
		public boolean isLess() {
			return less;
		}
	}
}
