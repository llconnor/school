(*****************************************************************************)
(* Larry Connor                                                              *)
(* Assignment 3                                                              *)
(* <Section>                                                                 *)
(* Tom Murphy		                                                     *)
(*****************************************************************************)


(*Compiler.Control.Print.printDepth := n;*)

(**************************************)
(* Problem 1: Dictionaries            *)
(**************************************)

(**************************************
 Problem 1 Invariants:
   lookup: must pass in a valid 'a
   insert: the new alpha must match up with the entry (otherwise the 'a would have must use)
   map: ('a -> 'b) must be a valid function (taking in 'a and returning 'b)
   exists: ('a entry -> bool) must be a valid function (taking in 'a entry and returning bool)
   all: ('a entry -> bool) must be a valid function (taking in 'a entry and returning bool)
**************************************)
signature DICT =
sig
   type key = string
   type 'a entry = key * 'a
   type 'a dict

   val empty : 'a dict 
   val lookup : 'a dict -> key -> 'a option 
   val map    : 'a dict -> ('a -> 'b) -> 'b dict 
   val insert : 'a dict * 'a entry -> 'a dict 
   val exists : 'a dict -> ('a entry -> bool) -> bool 
   val all    : 'a dict -> ('a entry -> bool) -> bool 
end

(******************)
(* Problem 1.1    *)
(******************)

structure BinaryTree :> DICT =
struct
   type key = string
   type 'a entry = key * 'a

   
   datatype 'a tree = 
       Empty
     | Node of 'a entry * 'a tree * 'a tree

   type 'a dict = 'a tree

   (* val empty : 'a dict *)
   val empty = Empty

  (* val lookup : 'a dict -> key -> 'a option *)
  fun lookup (adict:'a dict) (thekey:key) =
	let
	fun lookup' Empty (lookupkey:key) = NONE 
		| lookup' (Node((mykey, myalpha):'a entry, l, r):'a dict) (lookupkey:key) = 
			case String.compare(lookupkey, mykey) of
			LESS => lookup l (lookupkey)
			| EQUAL => SOME(myalpha)
			| GREATER => lookup r (lookupkey)
	in
		lookup' adict thekey
	end

(* val insert : 'a dict * 'a entry -> 'a dict *)
fun       insert (Empty, (k, al))                       = Node((k, al), Empty, Empty)
	| insert (Node((currkey, curralpha), Empty, r), (k, al))        = if (k = currkey) then (Node((k, al), Empty, r))
									else insert (r, (k, al))
	| insert (Node((currkey, curralpha), l, Empty), (k, al))        = if (k = currkey) then (Node((k, al), l, Empty))
									else insert (l, (k, al))
	| insert (Node((currkey, curralpha), l, r), (newkey, newalpha)) = 
	                                         case String.compare(currkey, newkey) of
	                                              LESS => insert (l, (newkey, newalpha))
	                                           | EQUAL => (Node((newkey, newalpha), l, r))
	                                         | GREATER => insert (r, (newkey, newalpha))

(* val map    : 'a dict -> ('a -> 'b) -> 'b dict *)
fun		map Empty x = Empty
		| map (Node((mykey, myalpha), l, r):'a dict) x = Node((mykey, x myalpha), map l x, map r x)

(* val exists : 'a dict -> ('a entry -> bool) -> bool *)
fun 	exists (Node(ent, l, r)) func = (func(ent)) orelse (exists l func) orelse (exists r func)
	| exists Empty func = false

(* val all    : 'a dict -> ('a entry -> bool) -> bool *)
fun	all (Node(ent, l, r)) func = (func(ent)) andalso (all l func) andalso (all r func)
	| all Empty func = false

end


(******************)
(* Problem 1.2    *)
(******************)

structure TwoThreeTree : DICT =
struct
   type key = string
   type 'a entry = key * 'a

   datatype 'a tree =
       Empty
     | TwoNode of 'a entry * ('a tree * 'a tree)
     | ThreeNode of ('a entry * 'a entry) * ('a tree * 'a tree * 'a tree)

   type 'a dict = 'a tree

   (* val empty : 'a dict *)
   val empty = Empty

   exception Something

  (* val lookup : 'a dict -> key -> 'a option *)
  fun lookup (Empty:'a dict) (lookupkey) = NONE
	| lookup (TwoNode((mykey, myalpha), (l, r)):('a dict)) (lookupkey) =
		(case String.compare(lookupkey, mykey) of
		LESS => lookup l (lookupkey)
		| EQUAL => SOME(myalpha)
		| GREATER => lookup r (lookupkey))
	| lookup (ThreeNode(((key1, alpha1):'a entry, (key2, alpha2):'a entry), (l:'a dict, m:'a dict, r:'a dict)):('a dict)) (lookupkey) =
		(case String.compare(lookupkey, key1) of
		LESS => lookup l (lookupkey)
		| EQUAL => SOME(alpha1)
		| GREATER => (case String.compare(lookupkey, key2) of
			LESS => lookup m (lookupkey)
			| EQUAL => SOME(alpha1)
			| GREATER => lookup r (lookupkey)))

  (* val map    : 'a dict -> ('a -> 'b) -> 'b dict *)
  fun		map Empty x = Empty
		| map (TwoNode((mykey, myalpha):'a entry, (l, r)):'a dict) x = TwoNode((mykey, x myalpha), (map l x, map r x))
		| map (ThreeNode(((key1, alpha1):'a entry, (key2, alpha2):'a entry), (l:'a dict, m:'a dict, r:'a dict)):('a dict)) x = ThreeNode(((key1, x alpha1), (key2, x alpha2)), (map l x, map m x, map r x))
   (* val insert : 'a dict * 'a entry -> 'a dict *)

   (* val exists : 'a dict -> ('a entry -> bool) -> bool *)
  fun 		exists (TwoNode(ent, (l, r))) func = (func(ent)) orelse (exists l func) orelse (exists r func)
		| exists Empty func = false
		| exists (ThreeNode((ent1, ent2), (l,m, r))) func = (func(ent1)) orelse (func(ent2)) orelse (exists l func) orelse (exists m func) orelse (exists r func) 

   (* val all    : 'a dict -> ('a entry -> bool) -> bool *)
  fun 		all (TwoNode(ent, (l, r))) func = (func(ent)) andalso (all l func) andalso (all r func)
		| all Empty func = false
		| all (ThreeNode((ent1, ent2), (l,m, r))) func = (func(ent1)) andalso (func(ent2)) andalso (all l func) andalso (all m func) andalso (all r func) 

fun       insert (Empty, (k, al))                       = TwoNode((k, al), (Empty, Empty))
	| insert (TwoNode((currkey, curralpha), (Empty, Empty)), (k, al)) =  (case String.compare(currkey, k) of
										LESS => ThreeNode(((currkey, curralpha), (k, al)), (Empty, Empty, Empty))
										| EQUAL => (TwoNode((k, al), (Empty, Empty)))
										| GREATER => ThreeNode(((k, al), (currkey, curralpha)), (Empty, Empty, Empty)))
	| insert (TwoNode((currkey, curralpha), (Empty, r)), (k, al))        = if (k = currkey) then (TwoNode((k, al), (Empty, r)))
									else insert (r, (k, al))
	| insert (TwoNode((currkey, curralpha), (l, Empty)), (k, al))        = if (k = currkey) then (TwoNode((k, al), (l, Empty)))
									else insert (l, (k, al))
	| insert (TwoNode((currkey, curralpha), (l, r)), (newkey, newalpha)) = 
	                                         (case String.compare(currkey, newkey) of
	                                              LESS => insert (l, (newkey, newalpha))
	                                           | EQUAL => (TwoNode((newkey, newalpha), (l, r)))
	                                         | GREATER => insert (r, (newkey, newalpha)))
	| insert (ThreeNode(((k1, a1), (k2, a2)), (l, m, r)), (k, al))= (case String.compare(k, k1) of
							LESS => insert(l, (k, al))
							| EQUAL => ThreeNode(((k, al), (k2, a2)), (l, m, r))
							| GREATER => (case String.compare(k, k2) of
								LESS => insert(m, (k, al))
								| EQUAL => ThreeNode(((k1, a1), (k, al)), (l, m, r))
								| GREATER => insert(r, (k, al))
							))
							
end

(**************************************)
(* Problem 2: Conjunctive Normal Form *)
(**************************************)

signature LOGIC =
sig
   datatype expr = 
       Var of string 
     | Not of expr
     | Or of expr * expr
     | And of expr * expr
     | Implies of expr * expr
     | Iff of expr * expr
   val toCNF : expr -> expr
   val removeIfs: expr -> expr
   val distributeNeg: expr -> expr
   val distributeOr: expr -> expr
end


(****************************)
(* Problem 2.1              *)
(****************************)


(*****************************************************************************

CHECK THESE 

1. Not(P Implies Q) => Not(Not(P) Or Q) => Not(Not(P)) And Not(Q) => P And Not(Q)

2. P Iff (Q And Not(R)) => (Not(P) Or (Q And Not(R)) And (P Or Not(Q And Not(R)))
=> (Not(P) Or (Q And Not(R)) And (P Or (Not(Q) Or Not(Not(R))))
=> (Not(P) Or (Q And Not(R)) And (P Or (Not(Q) Or (R)))
=> ((Not(P) Or Q) And (Not(P) or Not(R))) And ((P Or Not(Q)) And (P Or R))

3. (Not(A) Or Not(B)) Implies Not(C)
=> Not(Not(A) Or Not(B)) Or Not(C)
=> (Not(Not(A)) And Not(Not(B))) Or Not(C)
=> (A And B) Or Not(C)
=> (A Or Not(C)) And (B Or Not(C))

4. Not((A Or Not(B)) And (B Or C) And (Not(D) Or E))
=> Not(A Or Not(B)) Or Not(B Or C) Or Not(Not(D) Or E)
=> (Not(A) And Not(Not(B))) Or (Not(B) And Not(C)) Or (Not(Not(D)) And Not(E))
=> (Not(A) And B) Or (Not(B) And Not(C)) Or (D And Not(E))
=> ((Not(A) And B) Or Not(B)) And ((Not(A) And B) Or Not(C)) Or (D And Not(E))
=> ((Not(A) Or Not(B)) And (B Or Not(B)) (equals T)) And ((Not(A) Or Not(C)) And (B Or Not(C))) Or (D And Not(E))
=> ((Not(A) Or Not(B)) And (Not(A) Or Not(C) And (B Or Not(C))) Or (D And Not(E))
=> (D And Not(E)) Or ((Not(A) Or Not(B)) And (Not(A) Or Not(C) And (B Or Not(C)))
=> ((D And Not(E)) Or (Not(A) Or Not(B))) And ((D And Not(E)) Or (Not(A) Or Not(C))) And ((D And Not(E) Or (B Or Not(C)))
=> 

5. (A Implies (B And C)) Or (D Implies (E And F)) 
=> (Not(A) Or (B And C)) Or (Not(D) And (E And F))
=> ((Not(A) Or B) And (Not(A) Or C)) Or (Not(D) And E And F)
=> ((Not(A) Or B) Or (Not(D) And E And F)) And ((Not(A) Or C) Or (Not(D) And E And F))
=> ((Not(A) Or B Or Not(D)) And (Not(A) Or B Or E) And (Not(A) Or B Or F)) And ((Not(A) Or C Or Not(D)) And (Not(A) Or C Or E) And (Not(A) Or C Or F))

*****************************************************************************)

structure PropLogic :> LOGIC =
struct
   datatype expr = 
       Var of string 
     | Not of expr
     | Or of expr * expr
     | And of expr * expr
     | Implies of expr * expr
     | Iff of expr * expr
   infix Or And Implies Iff

   (****************************)
   (* Problem 2.2              *)
   (****************************)
   
   (* removeIfs: expr -> expr *)
	fun removeIfs (Var(x)) = Var(x)
	| removeIfs (exp1 And exp2) = removeIfs(exp1) And removeIfs(exp2)
	| removeIfs (Not(exp)) = Not(removeIfs(exp))
	| removeIfs (exp1 Or exp2) = removeIfs(exp1) Or removeIfs(exp2)
	| removeIfs (exp1 Implies exp2) = Not(removeIfs(exp1)) Or removeIfs(exp2)
	| removeIfs (exp1 Iff exp2) = (Not(removeIfs(exp1)) Or removeIfs(exp2)) And (removeIfs(exp1) Or Not(removeIfs(exp2)))
   (****************************)
   (* Problem 2.3              *)
   (****************************)
   
   (* distributeNeg: expr -> expr *)
   fun distributeNeg (Var(x)) = Var(x)
	| distributeNeg (exp1 And exp2) = distributeNeg(exp1) And distributeNeg(exp2)
   	| distributeNeg (exp1 Or exp2) = distributeNeg(exp1) Or distributeNeg(exp2)
	| distributeNeg (Not(Not(exp1))) = distributeNeg(exp1) 
	| distributeNeg (Not(exp1 Or exp2)) = distributeNeg(Not(exp1)) And distributeNeg(Not(exp2))
	| distributeNeg (Not(exp1 And exp2)) = distributeNeg(Not(exp1)) Or distributeNeg(Not(exp2))
	| distributeNeg (x) = x
   (****************************)
   (* Problem 2.4              *)
   (****************************)
   
   (* distributeOr: expr -> expr *)
   fun distributeOr (exp1 Or (exp2 And exp3)) = (distributeOr(exp1) Or distributeOr(exp2)) And (distributeOr(exp1) Or distributeOr(exp3))
	| distributeOr ((exp1 And exp2) Or exp3) = (distributeOr(exp1) Or distributeOr(exp3)) And (distributeOr(exp2) Or distributeOr(exp3))
	| distributeOr (exp1 And exp2) = distributeOr(exp1) And distributeOr(exp2)
	| distributeOr (exp1 Or exp2) = distributeOr(exp1) Or distributeOr(exp2)
	| distributeOr (Not(exp1)) = Not(distributeOr(exp1))
	| distributeOr (Var(x)) = Var(x)
	| distributeOr (x) = x
   
   (****************************)
   (* Problem 2.5              *)
   (****************************)
   
   (* toCNF: expr -> expr *)
   (* placeholder: *)
   val toCNF : expr -> expr = (fn p => p)
   fun toCNF(exp) = (distributeOr(distributeNeg(removeIfs(exp))))
end


(**************************************)
(* Problem 3: Regular Expressions     *)
(**************************************)

(* For problems 3.1-3.3 and 3.5, simply add your code to the following: *)

datatype regexp =
    Char of char
  | Epsilon
  | Empty
  | Underscore
  | Top
  | Concat of regexp * regexp
  | Union of regexp * regexp
  | Star of regexp
  | And of regexp * regexp
(* val acc : regExp -> char list -> (char list -> bool) -> bool *)
fun acc (Char(a)) (a1::s) k = if (a = a1) then k s else false
  | acc (Char(a)) (nil) k = false
  | acc (Concat(r1,r2)) s k = acc r1 s (fn s' => acc r2 s' k)
  | acc (Epsilon) s k = k s
  | acc (Union(r1,r2)) s k = acc r1 s k orelse acc r2 s k
  | acc (Empty) s k = false
  | acc (r as Star(r1)) s k = k s orelse
   acc r1 s (fn s' => if s = s' then false else acc r s' k)
  | acc (And(r1,r2)) s k = acc r1 s k andalso acc r2 s k
  | acc (Underscore) (a1::s) k = k s
  | acc (Underscore) nil k = k nil
  | acc (Top) s k = k s orelse
   acc Underscore s (fn s' => if s = s' then true else acc Underscore s' k)
(* val accept : regexp -> string -> bool *)
fun accept r s = acc r (String.explode s) List.null

(* some examples *)

(*val Ca = Char(#"a");
val Cb = Char(#"b");

val a1 = accept(And(Concat(Ca, Cb), Ca));
val a1 = accept (Concat(Ca,Ca)); (* aa *)
val a2 = accept (Star(Union(Ca,Cb))); (* (a+b)* *)
val a3 = accept (Concat(Concat(Concat(Star(Union(Ca,Cb)),Ca),Ca),
   	       Star(Union(Ca,Cb))));
         (* (a+b)*aa(a+b)* *)
val a4 = accept (Concat(Union(Ca,Epsilon),Star(Union(Cb,Concat(Cb,Ca)))));
         (* (a+1)(b+ba)* *)
*)



(******************)
(* Problem 3.4    *)
(******************)


(*****************************************************************************

Claim: The And constructor correctly implements the & operator.

Proof:
The And operator just means that the string has to be accepted in both strings.  This means that if
you run the string on both FSM's then (by inspection) the boolean value works the same as the And
operator.



*****************************************************************************)