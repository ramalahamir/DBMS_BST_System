### Assignment 03

#### Author Ramalah Amir 23i-2644 DS-C

### Player and Game DBMS Project

#### Overview

This project is a Database Management System (DBMS) for managing a dataset of players and the games they play. The system is implemented using Binary Search Trees (BST) to optimize data handling, including insertion, search, and deletion operations. The DBMS is designed to be used on the console and meets requirements for specific queries and actions specified by the company.

##### Features

###### Data Storage in Memory

###### Reads data from CSV files for players and games.

Stores data in memory as BST nodes for efficient access.
Uses a random seed based on roll number to selectively read player data.
Insertion

###### Adds players and games dynamically to the tree.

Checks for duplicate primary keys to avoid conflicts.
Search and Retrieval

###### Allows searching for players or games by primary key.

Returns the corresponding node for viewing or editing.
Deletion

- Supports deletion of nodes, ensuring no memory leaks.
  Save Data

- Saves the BST structure back to a CSV file using Preorder Traversal, preserving the tree structure.
  Show N Layers

###### Displays tree nodes up to N layers.

Provides a warning if N exceeds the tree depth.
Show Layer Number

###### Finds and displays the layer number of a node by its primary key.

Show Path

###### Displays the preorder path taken to find a node.

Edit Entry

- Allows editing of node data, including primary key adjustments, repositioning the node if needed.
  Top N Players

###### Finds players with the most games played.

Shows their Details

- Shows a player's details along with all games they’ve played.
  Has Played

- Efficiently checks if a player has played a specific game with time complexity 2 \_ log(n).

###### Seed Generation

The batch and roll number are combined to create a unique seed. This seed guides the data reading process: a random number (0 to 1000) is generated for each line. If this number is less than (44 x 10) + 100, the line is skipped; otherwise, it’s added to the BST.

###### Code Design and Logic

Each task was carefully designed to work dynamically, avoiding hard-coded solutions and allowing for future scalability. Only necessary comments were added to explain complex logic, keeping the code clean and efficient.

###### Time Complexity Analysis

Insertion, Search, Deletion: Average time complexity is O(log n) due to the BST structure.
Top N Players, Show Details, Has Played: Algorithms are optimized with custom structures or conditions to ensure efficient access within the required complexity.
Version Control
This project uses GitHub for version control. Frequent and meaningful commits were made to track progress and document changes in functionality or optimization.
