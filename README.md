# Smart_Recommendation_System

# Usage

After compile, assume you get an executable named main. Type “./main ./People.csv ./Ratings.csv -m” for movie database and “./main ./BBooks.csv ./BRatings.csv -b” for book database, in which the 4th argument in command line indicates the database type.

# Methods

For assigning algorithm to recommendation application, 0 is Manhattan, 1 is Euclidean, 2 is Pearson, 3 is Cosine. And any number greater than 3 is K-nearest neighbor. 4 means you just want the top neighbor, 5 means you want the top 2 neighbors, 6 means you want the top 3 neighbor, etc. In this application I’m using 6 which will give you the top 3. You can change the number smoothly in main.cpp.

# Running 

When running this application, it will read database first. Then it will keep asking you the name of user who wants recommendation and giving you the neighbors and recommendations for each of the five algorithm. For exiting the application, simply type “exit” instead of user name.

# Data Set

Download data sets here: https://drive.google.com/open?id=1mOjwegfEgL-g8iF0y7-k4I8oMs2PEsRh


