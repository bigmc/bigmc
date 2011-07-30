
fun consume 0 c r = (c,r)
  | consume m c ("0"::t) = consume (m-1) (c @ ["0"]) t
  | consume m c (l::t) = consume (m+1) (c @ [l]) t
  | consume m c [] = (c,[])

(* fun m (lvl, ctx, param) str pat = ... *)

fun m (~1, ctx, param) _ [] = (ctx, param, true)
  | m (~1, ctx, param) _ _ = (ctx, param, false)
  | m (n, ctx, param) [] [] = (ctx,param, true)
  | m (n, ctx, param) [] _ = (ctx,param, false)
  | m (n, ctx, param) _ [] = (ctx,param, true)
  | m (n, ctx, param) ("0"::t) ("0"::r) = m (n-1, ctx, param) t r
  | m (n, ctx, param) t ("-"::r) = 
  let
	val (param', rest) = consume 0 [] t
  in
	m (n-1, [], param @ param' @ ctx) rest r
  end
  | m (n, ctx, param) (a::t) (b::r) = 
	if a = b then (print ("Matched " ^ a ^ " = " ^ b ^ "\n"); m (n+1, ctx, param) t r)
	else (print ("No match: " ^ a ^ " != " ^ b ^ "\n"); m (n, a :: ctx, param) t (b::r))



val t = ["d","b","b","0","c","0","0","e","a","0","b","b","0","c","0","0","d","0","0","0"]
val p = ["b","b","0","c","0"];
val q = ["b","b","0","-","0"];

fun matchall [] [] = []
  | matchall [] l = []
  | matchall ("0"::t) l = matchall t l
  | matchall (h::t) l = (m (0,[],[]) (h::t) l) :: matchall t l

