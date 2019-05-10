cs104 hw8
This project adds a "recommend" command to the amazon system.
Heap is basically a priority queue.
The algorithm is basically:
	Let's say that the logged-in user is A, and we calculate the "rating" of product P to A as follows:

	Let S be the set of all other users who have rated P. (We only look at products P that A has not yet rated.)
	For each user B in S, let r(B,P) be the rating that B gave P, and s(B,A) the similarity between B and A.
	Let R(P) be the sum of (1-s(B,A))*r(B,P), over all users B in S.
	Let W be the sum of (1-s(B,A)) over all users B in S.
	The rating of P for A is now R(P)/W. If W=0, then also R(P)=0, and we define the rating as 0/0 := 0.