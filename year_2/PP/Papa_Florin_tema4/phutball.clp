
;verific daca am castigat
(defrule winGame
	(declare (salience 10))
	(world (ball ?x ?y) (limit ?z ?t) (id ?i) (moves $?m))
	(test (eq ?x (- ?z 1)))
=>			
	(assert (win (id ?i) (moves $?m))))
	
(defrule removeWorldsAfterWin
	(declare (salience 9))
	(win (id ?i) (moves $?))
	?w <- (world (limit ? ?) (ball ? ?) (men $?) (id ?i) (moves $?))
=>
	(retract ?w))
	
(defrule removeMenAfterWin
	(declare (salience 9))
	(win (id ?i) (moves $?))
	?m <- (removeMen ? ? ? ? ? ?i $?)
=>
	(retract ?m))

(defrule removePosAfterWin
	(declare (salience 9))
	(win (id ?i) (moves $?))
	?m <- (possible ? ? ? ? ? ?i $?)
=>
	(retract ?m))
		
(defrule removeWorldsAfterLoss
	(declare (salience -1))
	?w <- (world (limit ? ?) (ball ? ?) (men $?) (id ?) (moves $?))
=>
	(retract ?w))
	
(defrule removeMenAfterLoss
	(declare (salience -1))
	?m <- (removeMen $?)
=>
	(retract ?m))	
	
(defrule testUp
	(declare (salience 5))
	(world (id ?i) (ball ?x ?y) (men $?list1 ?m1 ?m2 - $?list2))
	(test (and 
			(eq ?m1 (- ?x 1))
			(eq ?m2 ?y)))
=>
	(assert (possible -1 0 ?x ?y  1 ?i $?list1 ?m1 ?m2 - $?list2)))

(defrule testDiagRightUp
	(declare (salience 5))
	(world (id ?i) (ball ?x ?y) (men $?list1 ?m1 ?m2 - $?list2))
	(test (and 
			(eq ?m1 (- ?x 1))
			(eq ?m2 (+ ?y 1))))
=>
	(assert (possible -1 1 ?x ?y 1 ?i $?list1 ?m1 ?m2 - $?list2)))
	
(defrule testRight
	(declare (salience 5))
	(world (id ?i) (ball ?x ?y) (men $?list1 ?m1 ?m2 - $?list2))
	(test (and 
			(eq ?m1 ?x)
			(eq ?m2 (+ ?y 1))))
=>
	(assert (possible 0 1 ?x ?y 1 ?i $?list1 ?m1 ?m2 - $?list2)))

(defrule testRightDown
	(declare (salience 5))
	(world (id ?i) (ball ?x ?y) (men $?list1 ?m1 ?m2 - $?list2))
	(test (and 
			(eq ?m1 (+ ?x 1))
			(eq ?m2 (+ ?y 1))))
=>
	(assert (possible 1 1 ?x ?y 1 ?i $?list1 ?m1 ?m2 - $?list2)))
	
(defrule testDown
	(declare (salience 5))
	(world (id ?i) (ball ?x ?y) (men $?list1 ?m1 ?m2 - $?list2))
	(test (and 
			(eq ?m1 (+ ?x 1))
			(eq ?m2 ?y)))
=>
	(assert (possible 1 0 ?x ?y 1 ?i $?list1 ?m1 ?m2 - $?list2)))
	
(defrule testLeftDown
	(declare (salience 5))
	(world (id ?i) (ball ?x ?y) (men $?list1 ?m1 ?m2 - $?list2))
	(test (and 
			(eq ?m1 (+ ?x 1))
			(eq ?m2 (- ?y 1))))
=>
	(assert (possible 1 -1 ?x ?y 1 ?i $?list1 ?m1 ?m2 - $?list2)))
	
(defrule testLeft
	(declare (salience 5))
	(world (id ?i) (ball ?x ?y) (men $?list1 ?m1 ?m2 - $?list2))
	(test (and 
			(eq ?m1 ?x)
			(eq ?m2 (- ?y 1))))
=>
	(assert (possible 0 -1 ?x ?y 1 ?i $?list1 ?m1 ?m2 - $?list2)))
	
(defrule testUpLeft
	(declare (salience 5))
	(world (id ?i) (ball ?x ?y) (men $?list1 ?m1 ?m2 - $?list2))
	(test (and 
			(eq ?m1 (- ?x 1))
			(eq ?m2 (- ?y 1))))
=>
	(assert (possible -1 -1 ?x ?y 1 ?i $?list1 ?m1 ?m2 - $?list2)))
	
(defrule incMove
	(declare (salience 5))
	?p <- (possible ?dir1 ?dir2 ?x ?y ?nr ?i $?list ?man1 ?man2 - $?list2)
	(test (and
			(eq ?man1 (+ ?x (* ?nr ?dir1)))
			(eq ?man2 (+ ?y (* ?nr ?dir2)))))
	(world (id ?i) (ball ?x ?y) (men $?list ?man1 ?man2 - $?list2))
=>
	(retract ?p)
	(assert (possible ?dir1 ?dir2 ?x ?y (+ ?nr 1) ?i $?list ?man1 ?man2 - $?list2)))

(defrule eliminateMen
	(declare (salience 6))
	?r <- (removeMen ?x ?y ?dir1 ?dir2 ?nr ?i $?list ?m1 ?m2 - $?list2)
	(test (> ?nr 0))
	(test (and 
			(eq ?m1 (+ ?x (* ?dir1 ?nr))) 
			(eq ?m2 (+ ?y (* ?dir2 ?nr)))))
	?w <- (world (ball ?x ?y) (men $?list ?m1 ?m2 - $?list2) (id ?i))
=>
	(retract ?r)
	(assert (removeMen ?x ?y ?dir1 ?dir2 (- ?nr 1) ?i $?list $?list2))
	(modify ?w (men $?list $?list2)))
	
(defrule eliminateMen2
	(declare (salience 6))
	?r <- (removeMen ?x ?y ?dir1 ?dir2 ?nr ?i ?m1 ?m2 -)
	(test (> ?nr 0))
	(test (and 
			(eq ?m1 (+ ?x (* ?dir1 ?nr))) 
			(eq ?m2 (+ ?y (* ?dir2 ?nr)))))
	?w <- (world (ball ?x ?y) (men ?m1 ?m2 -) (id ?i))
=>
	(retract ?r)
	(assert (removeMen ?x ?y ?dir1 ?dir2 (- ?nr 1) ?i))
	(modify ?w (men)))	
	
(defrule moveInit
	?p <- (possible ?dir1 ?dir2 ?x ?y ?nr ?i $?list)
	(world (limit ?l1 ?l2) (ball ?x ?y) (men $?list) (id ?i) (moves))
=>
	(retract ?p)
	(assert (removeMen (+ ?x (* ?dir1 ?nr)) (+ ?y (* ?dir2 ?nr)) (- 0 ?dir1) (- 0 ?dir2) (- ?nr 1) ?i $?list))
	(assert (world 
				(limit ?l1 ?l2) 
				(ball (+ ?x (* ?dir1 ?nr)) 
				(+ ?y (* ?dir2 ?nr))) 
				(men $?list) 
				(id ?i) 
				(moves ?x ?y - (+ ?x (* ?dir1 ?nr)) (+ ?y (* ?dir2 ?nr)) -))))
	
(defrule moveAfter
	?p <- (possible ?dir1 ?dir2 ?x ?y ?nr ?i $?list)
	(world (limit ?l1 ?l2) (ball ?x ?y) (men $?list) (id ?i) (moves ?a $?ls))
=>
	(retract ?p)
	(assert (removeMen (+ ?x (* ?dir1 ?nr)) (+ ?y (* ?dir2 ?nr)) (- 0 ?dir1) (- 0 ?dir2) (- ?nr 1) ?i $?list))
	(assert (world 
				(limit ?l1 ?l2) 
				(ball (+ ?x (* ?dir1 ?nr)) (+ ?y (* ?dir2 ?nr))) 
				(men $?list) 
				(id ?i) 
				(moves ?a $?ls (+ ?x (* ?dir1 ?nr)) (+ ?y (* ?dir2 ?nr)) -))))	