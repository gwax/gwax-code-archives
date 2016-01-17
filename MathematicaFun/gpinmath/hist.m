(* ::Package:: *)

<<Graphics`Graphics`

<<Graphics`Animation`

Run["copy pop.log+pop.m pop.ful /Y > nul"]

<<pop.ful

popfit=MapThread[List, pop][[2]]

Histogram[x_, opts___]:=
	Module[{data, fl, figs},
		data=Table[0, {10}];
		figs=Map[Floor, popfit[[x+1]]*10];
		figs=Map[If[#==0, 1, #]&, figs];
		Map[(data[[#]]++)&, figs];
		BarChart[data, BarLabels->Table[i, {i, 0, 0.9, 0.1}],
				PlotRange->{{0, 11}, {0, PopulationSize}},
				PlotLabel->StringJoin["Generation ", ToString[x]],
				opts]
		]

HistTable:=Table[Histogram[i, DisplayFunction->Identity],
				{i, 0, Length[pop]-1}]

AnimateHist:=ShowAnimation[HistTable];
