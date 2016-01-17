(* ::Package:: *)

ShowSample:=ListPlot[MapThread[List, {XPoints, YPoints}]]

ShowCurve:=
	Module[
		{t},
		t=MapThread[List, SolutionSet];
		ListPlot[MapThread[List, {Join[t[[1]], t[[1]]],
		Join[t[[2]], t[[4]]]}],
		PlotRange->{{0, 51}, {0, 1}}]
		]

ShowSolution:=Plot[Solution /. XTrans, {x, -2, 2}]

ShowFit:=
	Show[ShowSample, ShowSolution,
		PlotRange->{{-2, 2}, {-2, 10}},
		PlotLabel->Solution /. XTrans, AxesLabel->{x, ""},
		Frame->True
		]

Stats[s_String]:=
	Module[{},
		Display[StringJoin[s, ".sam"], ShowSample];
		Display[StringJoin[s, ".sol"], ShowSolution];
		Display[StringJoin[s, ".fit"], ShowFit];
		Display[StringJoin[s, ".scu"], ShowCurve];
		]
