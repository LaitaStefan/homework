#|
Interrogates a database given the specified query. Returns a list of substitutions.
|#

;; utility functions

#|
Checks if the provided value is a variable
|#
(define (variable? sym)
  (and (symbol? sym) (equal? (string-ref (symbol->string sym) 0) #\?)))
#| functie care verifica daca exista match pe partea de final // verifica 2 liste elem cu elem|#
(define match-final
  (λ(x1 x2)
    (if (list? x1)
        (if (list? x2)
            (if (= (length x1) (length x2))
                (evaluate (map match-final x1 x2))
                #f)
            #f)
        (if (variable? x1)
            #t
            (if (list? x2)
                #f
                (if (checkEqual x1 x2)
                    #t
                    #f))))))
#| functie care verifica daca exista match|#
(define match
  (λ(L1 L2)
    (if (or (variable? (car L1)) (string=? (symbol->string (car L1)) (symbol->string (car L2))))
        (if (string=? (symbol->string (cadr L1)) (symbol->string (cadr L2)))
            (match-final (cddr L1) (cddr L2))
            #f)
        #f)))      

;intoarce true daca o lista contine cel putin un element #f
(define evaluate
  (λ(L)
    (if (null? L)
        #t
        (if (list? (car L))
            (and (evaluate (car L)) (evaluate (cdr L)))
            (and (car L) (evaluate (cdr L)))))))

;intoarce valoarea unui simbol daca exista in lista L, null altfel
(define valueOf
  (λ(x L)
    (if (null? L)
        '()
        (if (string=? (symbol->string (caar L)) (symbol->string x))
            (cadar L)
            (valueOf x (cdr L))))))

;evalueaza expresia if
(define evaluateIfStatement
  (λ(L1 L2)
    (if (= (deepLength (replaceVariables L1 L2)) (deepLength L1))
        (eval (cadr (replaceVariables L1 L2)))
        #f)))

(define deepLength
  (λ(L)
    (if (null? L)
        0
        (if (list? (car L))
            (+ (deepLength (car L)) (deepLength (cdr L)))
            (+ 1 (deepLength (cdr L)))))))

;inlocuieste in expresiile mele variabilele cu valori efective
(define replaceVariables
  (λ(L1 L2)
    (if (null? L1)
        '()
        (if (list? (car L1))
            (cons (replaceVariables (car L1) L2) (replaceVariables (cdr L1) L2))
            (if (variable? (car L1))
                (if (null? (valueOf (car L1) L2))
                    (error "Unbound Variable Exception")
                    (cons (valueOf (car L1) L2) (replaceVariables (cdr L1) L2)))
                (cons (car L1) (replaceVariables (cdr L1) L2)))))))

;verifica egalitatea pentru simboluri/numere
(define checkEqual
  (λ(x y)
    (if (number? x)
        (if (and (number? y) (= x y))
            #t
            #f)
        (if (and (symbol? y) (string=? (symbol->string x) (symbol->string y)))
            #t
            #f))))


;pune in lista acc bind-urile(liste de liste de perechi) pentru o singura runda de query
(define createBinds
  (λ(query db acc)
    (if (null? db)
        acc
        (if (match query (car db))
            (createBinds query (cdr db) (cons (getVariables query (car db)) acc))
            (createBinds query (cdr db) acc)))))

;scoate perechile variabila-valoare din 2 expresii care au facut match
(define getVariables
  (λ(from fact)
    (if (null? from)
        null
        (if (list? (car from))
            (append (getVariables (cdr from) (cdr fact)) (getVariables (car from) (car fact)))
            (if (variable? (car from))
                (cons (list (car from) (car fact)) (getVariables (cdr from) (cdr fact)))
                (getVariables (cdr from) (cdr fact)))))))

;face reuniunea a 2 liste de perechi variabila-valoare
(define listReunion
  (λ(L1 L2)
    (if (listConflicts L1 L2)
        null
        (noReplAppend L1 L2 L1))))

;face append intre 2 liste de perechi
(define noReplAppend
  (λ(L1 L2 acc)
    (if (null? L2)
        acc
        (if (null? (valueOf (caar L2) L1))
            (noReplAppend L1 (cdr L2) (append acc (list (car L2))))
            (noReplAppend L1 (cdr L2) acc)))))

;face reuniunea unui bind cu o lista de perechi
(define bind-pairReunion
  (λ(bind L)
    (if (null? bind)
        null
        (cons (listReunion (car bind) L) (bind-pairReunion (cdr bind) L)))))

;face reuniunea a 2 bind-uri
(define bindReunion
  (λ(b1 b2)
    (if (null? b2)
        null
        (eliminate-null (append (bind-pairReunion b1 (car b2)) (bindReunion b1 (cdr b2)))))))

;elimina elementele vide dintr-o lista
(define eliminate-null
  (λ(L)
    (if (null? L)
        null
        (if (null? (car L))
            (eliminate-null (cdr L))
            (cons (car L) (eliminate-null (cdr L)))))))

;verifica daca intre 2 liste sunt conflicte
(define conflicts
  (λ(L1 x)
    (if (null? L1)
        #f
        (if (null? (valueOf (car x) L1))
            #f
            (if (equalRec (list (valueOf (car x) L1)) (list (cadr x)))
                #f
                #t)))))
(define listConflicts
  (λ(L1 L2)
    (if (or (null? L2) (null? L1))
        #f
        (if (conflicts L1 (car L2))
            #t
            (listConflicts L1 (cdr L2))))))

;aplicarea if pe toate bind-urile
(define ifCondition
  (λ(binds L)
    (if (null? binds)
        null
        (if (evaluateIfStatement L (car binds))
            (cons (car binds) (ifCondition (cdr binds) L))
            (ifCondition (cdr binds) L)))))
;verifica daca o lista contine conditia if
(define checkIf
  (λ(L)
    (if (null? L)
        #f
        (if (or (variable? (car L)) (list? (car L)))
            #f
            (if (string=? (symbol->string (car L)) "if")
                #t
                #f)))))

;rezolva un query
(define generateBinds
  (λ(query db acc)
    (if (or (null? query) (null? acc))
        acc
        (if (checkIf (car query))
            (generateBinds (cdr query) db (ifCondition acc (car query)));prost aici
            (letrec ((rez (createBinds (car query) db '())) (reunion (bindReunion acc rez)))
              (if (or (null? rez) (null? reunion))
                  null
                  (generateBinds (cdr query) db reunion)))))))

;selecteaza campurile cerute
(define selectOne
  (λ(sym binds)
    (if (null? (valueOf sym binds))
        (error "Unbound Variable Exception")
        (list sym (valueOf sym binds)))))

(define selectMultiple
  (λ(syms)
    (λ(binds)
      (if (null? syms)
          null
          (cons (selectOne (car syms) binds) ((selectMultiple (cdr syms)) binds))))))

(define selectAll
  (λ(syms binds)
    (map (selectMultiple syms) binds)))

;verifica daca exista conflicte la nivelul unui bind
(define equalRec
  (λ(L1 L2)
    (if (null? L1)
        (if (null? L2)
            #t
            #f)
        (if (null? L2)
            #f
            (if (list? (car L1))
                (if (list? (car L2))
                    (and (equalRec (car L1) (car L2)) (equalRec (cdr L1) (cdr L2)))
                    #f)
                (if (checkEqual (car L1) (car L2))
                    (equalRec (cdr L1) (cdr L2))
                    #f))))))

;verifica egalitatea primei valori dintre 2 perechi
(define equalFirst
  (λ(pair1 pair2)
    (if (checkEqual (car pair1) (car pair2))
        #t
        #f)))
;intoarce true daca 2 perechi nu sunt identice si false alftel
(define notEqualPairs
  (λ(pair1 pair2)
    (if (and (checkEqual (car pair1) (car pair2)) (equalRec (cdr pair1) (cdr pair2)))
        #f
        #t)))

;verifica daca intr-o lista de perechi exista conflicte
(define detectConflicts
  (λ(L1 L2 init)
    (if (null? L1)
        init
        (if (null? L2)
            (detectConflicts (cdr L1) (cdr L1) init)
            (if (and (equalFirst (car L1) (car L2)) (notEqualPairs (car L1) (car L2)))
                '()
                (detectConflicts L1 (cdr L2) init))))))

;aplica detectConflicts pe fiecare element al unui bind
(define getFinalBinds
  (λ(binds)
    (if (null? binds)
        null
        (cons (detectConflicts (car binds) (car binds) (car binds)) (getFinalBinds (cdr binds))))))

(define ask
  (λ(db L)
    (if (null? (cadadr L))
        '(())
        (selectAll (cadadr L) (eliminate-null (getFinalBinds (generateBinds (cdar L) db '(())) ))))))