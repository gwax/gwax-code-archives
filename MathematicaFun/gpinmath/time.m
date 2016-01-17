(* ::Package:: *)

Time[x_ Second /; x>=3600, Stuff___] :=
	Module[{h, m, s}, s = x; h = Floor[s/3600]; s -= h*3600; m = Floor[s/60];
		s -= m*60; Print[Stuff, h, " Hours, ", m, " Minutes, ", s, " Seconds"]]

Time[x_ Second /; x>=60, Stuff___] :=
	Module[{m, s}, s = x; m = Floor[s/60]; s -= m*60;
		Print[Stuff, m, " Minutes, ", s, " Seconds"]]

Time[x_ Second, Stuff___]:=
	Print[Stuff, x, " Seconds"]
