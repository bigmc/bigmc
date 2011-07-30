datatype b = 
	Pre of string * b
|	Par of b * b
|	Nil

fun pp (Pre(s,b)) = s ^ "." ^ pp b
  | pp (Par(a,b)) = "(" ^ pp a ^ " || " ^ pp b ^ ")"
  | pp Nil = "nil"

fun newRoot NONE r = SOME r
  | newRoot r _ = r

fun findMatches (r,Nil,[]) = (print "Match found\n"; [r])
  | findMatches (r,Nil,[Nil]) = (print "Match found\n"; [r])
  | findMatches (r,Nil,p) = (print ("No match: Nil against " ^ (String.concatWith "," (map pp p)) ^ "\n") ; [])
  | findMatches (r,p,[Nil]) = (print ("No match: " ^ pp p ^ " against [Nil]\n"); [])
  | findMatches (r,r' as Pre(k,p), [Pre(j,q)]) = 
	if k = j then findMatches'(newRoot r r', p, [q]) @ findMatches'(NONE,p,[Pre(j,q)]) 
	else findMatches(NONE,p,[Pre(j,q)])
  | findMatches (r,Pre(k,p),l) = findMatches'(NONE,p,l)
  | findMatches (r,a as Par(p,q),b as [Par(r1,r2)]) = 
	findMatches(r,a,[r1,r2]) @ findMatches'(r,a,[r2,r1]) @
	findMatches(NONE,p,b) @ findMatches'(NONE,q,b)
  | findMatches (r,Par(p,q),[r1,r2]) = 
    let
	val m1 = findMatches'(r,p,[r1])
    in
	if length m1 > 0 then m1 @ findMatches'(r,q,[r2]) else []
    end
  | findMatches (NONE,Par(p,q),[r1]) = findMatches'(NONE,p,[r1]) @ findMatches'(NONE,q,[r1])
  | findMatches (SOME r,Par(p,q),[r1]) = findMatches'(SOME r,p,[r1]) @ findMatches'(SOME r,q,[r1]) @ 
					findMatches'(NONE,p,[r1]) @ findMatches'(NONE,q,[r1])
  | findMatches (_,p,r) = raise Fail ("Unhandled case while matching: " ^ pp p ^ " against redex [" ^ (String.concatWith "," (map pp r)) ^ "]")
and findMatches' (NONE,p,q) = (print ("Matching: NONE, " ^ pp p ^ " with [" ^ (String.concatWith "," (map pp q)) ^ "]\n"); findMatches(NONE,p,q))
  | findMatches' (SOME r,p,q) = (print ("Matching: Root: " ^ pp r ^ ", " ^ pp p ^ " with [" ^ (String.concatWith "," (map pp q)) ^ "]\n"); findMatches(SOME r,p,q))

val t1 = Par(Pre("send",Pre("recv",Nil)),Pre("recv",Pre("send",Nil)))
val t2 = Pre("a",Pre("b",Pre("c",Pre("d",Nil))))
val t3 = Pre("c",Pre("d",Pre("c",Pre("d",Nil))))
val r1 = [Pre("c",Pre("d",Nil))]

val t4 = Pre("a",Par(Pre("b", Nil), Pre("c", Nil)))
val r2 = [Pre("a",Par(Pre("b", Nil), Pre("c", Nil)))]
val t5 = Par(Pre("a",Nil),Pre("a",Par(Pre("b", Nil), Pre("c", Nil))))

fun fm t r = findMatches'(NONE,t,r)
