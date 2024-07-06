7/7/2024 update :
added cmf 
added update_count which allows to adjust the count of an instance of an element in the set
have fun!
7/5/2024 big updates :
1-updated the class to be a multi-set or a bag 
  -now it allocates memory for unique elements only and for duplicates are stored in a counter for each node
  -fixed the issue where if you add sorted array of duplicates where (previously the set would contain duplicates) 
  -now since it's a bag it stores the unique elements only and increase count for each dupicate (aka you won't find     
  duplicates scattered around the bst they are stored in one location)

2-added new prob_draw_no_replace and prob_draw_replace they calculate the probability of a certain event to be picked from a sample space when no replace and replace respectively (check the documentation of the header)

3-now prob and prob_cond calculate probability if sample space contains the event and the probability in that function is size of event/size of sample space 

4-updated a lot of base functions like intersection,unite,diff and more to handle the multi-set and removed some bugs along the path (check prob.h) for more context 

5-added average,median,pmf,and weighted average (the class had to be updated to be a multi-set to allow for these calcuations) (for them to make since for example in a normal set if u get pmf the array is filled with 1/n which isn't the most useful calculation but now it has more power)

6-added ability to insert an element with a specific count (for example insert(5,25) inserts 25 instances of 5
in one O(logn) instead of 25*O(logn) 

7-the bst had a lot of updates one of them was adding a constructor to initialize a tree with an array of pointers where each element is an address to an element of an already created bst and since they are collected using euler-tour they are inorder to it constructs the new bst efficiently in O(N) with less wastage of space unlike the previous implementation that required to allocate a lot of memory when collecting elements from the set during the tour

8-euler tour section had a huge update,documentation is available in prob.h 

9-let me know if something isn't correct or some results aren't correct 

10-feel free to use any code :)

11-Just added those aswell variance ,standard_deviation ,range 

7/2/2024 update :
fixed a bug that caused an open-loop when removing duplicates from a the set
now remove duplicates works fine 

6/29/2024 update:
-fixed a bug in remove duplicates
6/25/2024 update :
this is the start of the project full documentation can be found in the header (prob.h) 
if you want to find the full bst implementation check out this ->[https://github.com/Andrew20371160/File-IO-With-Binary-Trees/tree/main/Custom%20Database%20Creator]
feel free to use any code :)


