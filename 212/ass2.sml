(*****************************************************************************)
(* Larry L. Connor                                                           *)
(* Assignment 2                                                              *)
(* Section D                                                                 *)
(* Tom Murphy		                                                     *)
(*****************************************************************************)

(**************************************)
(* Problem 1: Fibonacci Trees         *)
(**************************************)

(******************)
(* Problem 1.1    *)
(******************)

datatype fibTree = Young | Adult | Node of fibTree * fibTree

(*val nextFibTree : fibTree -> fibTree*)


fun nextFibTree Young = Adult
| nextFibTree Adult = Node(Young, Adult)
| nextFibTree(Node(l,r)) = Node(nextFibTree(l), nextFibTree(r))

fun createFibTree(1) = Young
| createFibTree(2) = Adult
| createFibTree(n) = nextFibTree(createFibTree(n-1))(*,nextFibTree(tree))*)

(* int n > 0*)
(* val createFibTree : int -> fibTree *)


(******************)
(* Problem 1.2    *)
(******************)

(* int n > 0*)

(* val createFibTree' : int -> fibTree *)
fun createFibTree' (1) = Young(*nextFibTree(tree)*)
| createFibTree'(2) = Adult(*Node(tree, nextFibTree(tree))*)
| createFibTree'(n) = Node(createFibTree(n - 2), createFibTree(n - 1))

(******************)
(* Problem 1.3    *)
(******************)

(* val countFibTreeLeaves : fibTree -> int*)
fun countFibTreeLeaves(Node(l,r)) = countFibTreeLeaves(l) + countFibTreeLeaves(r)
| countFibTreeLeaves(Young) = 1
| countFibTreeLeaves(Adult) = 1

(**************************************)
(* Problem 2: Tiling With Trominoes   *)
(**************************************)

(* the orientation is the direction in which the tromino's "arrow" points *)
datatype orientation = NW | NE | SW | SE
type tromino = int * int * orientation


(******************)
(* Problem 2.2    *)
(******************)

(* val translate : tromino list -> (int * int) -> tromino list *)
  fun translate (nil ,i1, i2) = nil
	| translate ((x, y, orient)::remain, i1, i2) = (x + i1, y + i2, orient)::translate(remain, i1, i2)

(******************)
(* Problem 2.3    *)
(******************)

(* val flip : tromino list -> int -> tromino list *)

  fun flip (nil, i1) = nil
	| flip ((x,y:int,orient)::remain, i1) = if(orient = SE) then (i1 - x + 1, y, SW)::flip(remain, i1)
						else if (orient = NE) then (i1 - x + 1, y, NW)::flip(remain, i1)
						else if (orient = NW) then (i1 - x + 1, y, NE)::flip(remain, i1)
						else
							(i1 - x + 1, y, SE)::flip(remain,i1)

(* val invert : tromino list -> int -> tromino list *)

  fun invert(nil, i1) = nil
	| invert((x,y, orient)::remain, i1) = if(orient = SE) then (x, i1 - y + 1, NE)::invert(remain,i1)
						else if (orient = SW) then (x, i1 - y + 1, NW)::invert(remain,i1)
						else if (orient = NE) then (x, i1 - y + 1, SE)::invert(remain, i1)
						else
							(x, i1 - y + 1, SW)::invert(remain, i1)



(******************)
(* Problem 2.4    *)
(******************)

(* val tile : int -> tromino list *)

(*make2of2 makes a 4x4 square of trominos (I use this as my base case) *)
  fun make2of2(0,0) = nil
	| make2of2(x,y) = [(x,y,SW), (x+3,y,SE), (x,y+3,NW), (x+3, y+3,NE), (x+1, y+1,SW)]

  fun tile(1) = ([(1,1,NW)])
	| tile(2) = make2of2(1,1)
	| tile(n) = tile(n-1)@flip(tile(n-1), 8)@invert(tile(n-1), 8)@invert(flip(tile(n-1), 8), 8)


(**************************************)
(* Problem 3: Circuits                *)
(**************************************)

signature CIRCUIT = sig

  (* basic circuits *)
  datatype circuit =
      VAR of string
    | CONST of bool
    | NOT of circuit
    | AND of circuit * circuit
    | OR of circuit * circuit
    | NAND of circuit * circuit
    | NOR of circuit * circuit
    | XOR of circuit * circuit

  (* circuits with only nand gates *)
  datatype nandcircuit =
      VAR_ of string
    | CONST_ of bool
    | NAND_ of nandcircuit * nandcircuit

  (* an mapping of variables to boolean values *)
  type environment = (string * bool) list

  (* raised if the evaluation of a circuit references an undefined variable *)
  exception UndefinedVariable

  (* operations on circuits *)
  val vars : circuit -> string list
  val evaluate : (circuit * environment) -> bool
  val transform : circuit -> environment -> bool
  val toNand : circuit -> nandcircuit

  (* extra credit: simplifying circuits *)
  val simplify : circuit -> circuit

end

structure Circuit :> CIRCUIT = struct


(******************)
(* Problem 3.1    *)
(******************)

  (* the abstract circuit type *) 
  datatype circuit =
      CONST of bool
    | VAR of string
    | NOT of circuit
    | AND of circuit * circuit
    | OR of circuit * circuit
    | NAND of circuit * circuit
    | NOR of circuit * circuit
    | XOR of circuit * circuit


(*  structure circuit :> CIRCUIT = struct*)
  (* an mapping of variables to boolean values *)
  type environment = (string * bool) list

  (* raised if the evaluation of a circuit references an undefined variable *)
  exception UndefinedVariable
  
  

  fun vars(NOT(l)) = vars(l)
	| vars(AND(l,r)) = vars(l)@vars(r)
	| vars(OR(l,r)) = vars(l)@vars(r)
	| vars(NAND(l,r)) = vars(l)@vars(r)
	| vars(NOR(l,r)) = vars(l)@vars(r)
	| vars(XOR(l,r)) = vars(l)@vars(r)
	| vars(CONST true) = nil
	| vars(CONST false) = nil
	| vars(VAR x) = x::nil

  fun evaluate(CONST (true),_) = true
	| evaluate(CONST (false), _) = false
	| evaluate(VAR x, (l, k)::m) = if(l = x) then k
					else evaluate(VAR x, m)
	| evaluate(VAR x, nil) = raise UndefinedVariable (*if there is no list along with the variable*)
	| evaluate(AND(l,r), _) = evaluate(l, nil) andalso evaluate(r, nil)
	| evaluate(OR(l,r), _) = evaluate(l, nil) orelse evaluate(r, nil)
	| evaluate(NAND(l,r), _) = not(evaluate(l, nil) andalso evaluate(r, nil))
	| evaluate(NOR(l,r), _) = not(evaluate(l, nil) orelse evaluate(r, nil))
	| evaluate(NOT(l), _) = not(evaluate(l,nil))
	| evaluate(XOR(l,r), _) = (evaluate(l, nil) andalso (not(evaluate(r, nil)))) orelse (evaluate(r, nil) andalso (not(evaluate(l, nil))))
	

  fun transform c e = evaluate(c,e)

  (* val vars : circuit -> string list *)

  (* val evaluate : (circuit * environment) -> bool *)

  (* val transform : circuit -> environment -> bool *)


(******************)
(* Problem 3.2    *)
(******************)

  (* a nand only circuit *)
  datatype nandcircuit =
      CONST_ of bool
    | VAR_ of string
    | NAND_ of nandcircuit * nandcircuit

  fun toNand (NAND(l,r)) = NAND_(toNand(l),toNand(r))
	| toNand (CONST(true)) = CONST_ (true)
	| toNand (CONST(false)) = CONST_ (false)
	| toNand (NOT(l)) = NAND_(toNand(l),toNand(l))
	| toNand (AND(l,r)) = NAND_(NAND_(toNand(l), toNand(r)), NAND_(toNand(l), toNand(r)))
	| toNand (OR(l,r)) = NAND_(NAND_(toNand(l), toNand(l)), NAND_(toNand(r), toNand(r)))
	| toNand (NOR(l,r)) = NAND_(NAND_(NAND_(toNand(l), toNand(l)), NAND_(toNand(r), toNand(r))), NAND_(NAND_(toNand(l), toNand(l)), NAND_(toNand(r), toNand(r))))
	| toNand (XOR(l,r)) = NAND_(NAND_(toNand(l), NAND_(toNand(r), toNand(r))), NAND_(NAND_(toNand(l), toNand(l)), toNand(r)))
	| toNand (VAR x) = VAR_ (x)
(*   val toNand : circuit -> nandcircuit *)


(******************)
(* Problem 3.3    *)
(******************)

  (* extra credit: simplifying circuits *)
(*  fun simplify x = x*)

  fun simplify(AND(CONST true, CONST true)) = CONST true
	| simplify(AND(CONST true, x)) = x
	| simplify(AND(x, CONST true)) = x
	| simplify(AND(x, CONST false)) = CONST false
	| simplify(AND(CONST false, x)) = CONST false
	| simplify(OR(x, CONST false)) = x
	| simplify(OR(CONST false, x)) = x
	| simplify(OR(CONST true, x)) = CONST true
	| simplify(OR(x, CONST true)) = CONST true
	| simplify(NOR(x, CONST true)) = CONST false
	| simplify(NOR(CONST true, x)) = CONST false
	| simplify(NOR(CONST false, x)) = NOT(x)
	| simplify(NOR(x, CONST false)) = NOT(x)
	| simplify(NAND(x, CONST false)) = CONST true
	| simplify(NAND(CONST false, x)) = CONST true
	| simplify(NAND(x, CONST true)) = NOT(x)
	| simplify(NAND(CONST true, x)) = NOT(x)
	| simplify(AND(x,y)) = if(x = y) then x else AND(x,y)
	| simplify(OR(x,y)) = if(x = y) then x else OR(x,y)
	| simplify(NOT(AND(x,y))) = NAND(x,y)
	| simplify(NOT(OR(x,y))) = NOR(x,y)
	| simplify(NOT(NAND(x,y))) = AND(x,y)
	| simplify(NOT(NOR(x,y))) = OR(x,y)
	| simplify(NOT(NOT(x))) = x
	| simplify(x) = x
end


