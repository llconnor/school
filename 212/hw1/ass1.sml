fun bincoef(n, 0) = 1
  | bincoef(n, k) = if n = k then 1
                            else bincoef(n-1, k-1) + bincoef(n-1,k)

fun den(1) = 1
	| den(L) = L * den(L - 1)

fun num(n,q) = if (q = n) then n
		else q*num(n,q+1)


fun bincoef2(n,0) = 1
	| bincoef2(n, k) = if(n=k) then 1
			else if (k > (n-k)) then (num(n, k + 1) div den(n-k))
			else (num(n, (n-k) + 1) div den(k))

fun hgt(a, b, n, 0) = 0.0
  | hgt(a, b, n, c) = ((Real.fromInt(b)-Real.fromInt(a)) / Real.fromInt(n)) * Math.cos(Real.fromInt(a) + ((Real.fromInt(b) - Real.fromInt(a)) / Real.fromInt(n)) * Real.fromInt (c)) + hgt(a, b, n, c - 1)

fun rectangles(a, b, n) = hgt(a, b, n, n - 1)