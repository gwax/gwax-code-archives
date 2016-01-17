(* ::Package:: *)

ClearAttributes[Divide, Protected]
Divide[_, 0]:=1
SetAttributes[Divide, Protected]

ClearAttributes[Log, Protected]
Log[0]:=0
Log[x_ /; x<0]:=Log[-x]
Log[E^x_]:=x
SetAttributes[Log, Protected]

ClearAttributes[Power, Protected]
Power[0, -1]:=1
SetAttributes[Power, Protected]
