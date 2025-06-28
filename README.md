# Player and Game DBMS Project
Author: Ramalah Amir

# 📌 Overview
This project implements a Database Management System (DBMS) for managing data on players and the games they play, using Binary Search Trees (BSTs) for efficient in-memory data handling. It is a console-based application designed to support insertion, search, deletion, and various queries, as per functional requirements provided by the company.

# ⚙️ Features
## 🔍 Data Handling & Storage
- Reads player and game data from CSV files.
- Stores records in memory using BST nodes for optimized access.
- Uses a randomized seed (based on batch + roll number) to selectively filter player data during load.

## ➕ Insertion
- Dynamically inserts players and games into the BST.
- Detects and prevents insertion of duplicate primary keys.

## 🔎 Search & Retrieval
- Allows fast search of players/games via primary key.
- Returns corresponding node data for display or editing.

## ❌ Deletion
- Supports deletion of any node while ensuring no memory leaks.

## 💾 Save Data
- Saves BST data back to CSV using Preorder Traversal, maintaining structure for reloading.

## 🌳 Show N Layers
- Displays all nodes up to N levels in the BST.
- Alerts if N exceeds current tree depth.

## 📍 Show Layer Number
- Finds the layer/depth of a node based on its primary key.

## 📐 Show Path
- Displays the preorder traversal path taken to reach a specific node.

## 📝 Edit Entry
- Supports full editing of node data.
- Handles changes in primary key by repositioning the node as needed.

## 🏆 Top N Players
- Identifies and displays the players with the most games played.

## 🕹️ Show Details
- Shows a player's full details and all games they’ve played.

## ❓ Has Played
- Checks if a player has played a specific game using 2 × log(n) time complexity.

## 🔢 Seed Generation Logic
- A seed is generated using the formula:
- (Batch * 100) + Roll Number
- This guides a randomized filtering process:
- For each row, generate a number from 0 to 1000.
- If the number is less than (44×10) + 100, skip the entry.
- Otherwise, include it in the BST.

## 🧠 Code Design Philosophy
- Modular and dynamic design (no hardcoding).
- Comments added only where logic is non-trivial.
- Scalability and clean structure were prioritized.

## ⏱️ Time Complexity
- Insertion, Search, Deletion: O(log n) (avg case with BSTs)
- Top N Players, Show Details, Has Played:
- Uses optimized logic and custom traversal strategies.
- Has Played: Achieves 2 × log(n) by:
- Searching for the player → log(n)
- Searching within the player’s game list → log(n) (m ≤ n)
- Total: ≈ 2 × log(n)
