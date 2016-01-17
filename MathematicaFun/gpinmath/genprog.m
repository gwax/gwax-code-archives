(* ::Package:: *)

Get["time.m"]
Get["xtradefs.m"]

(* Terminals *)
(* Functions *)
(* Parameters *)

MaxComplexity=50

(* Generate random expression *)
GenerateNormal[d_]:=
	Module[
		{r, Poss, PossPar},
		If[
			d>1,
			Poss=Join[Functions, Terminals];
			PossPar=Parameters,
			Poss=Terminals;
			PossPar={}
			];
		While[
			Length[PossPar]<Length[Poss],
			PossPar=Append[PossPar,0]
			];
		r=Random[Integer, {1, Length[Poss]}];
		Switch[
			PossPar[[r]],
			0,
			Poss[[r]],
			1,
			Poss[[r]][Generate[d-1]],
			2,
			Poss[[r]][Generate[d-1], Generate[d-1]],
			3,
			Poss[[r]][Generate[d-1], Generate[d-1], Generate[d-1]]
			]
		]

Generate[d_]:=
	Module[
		{y},
		y=GenerateNormal[d];
		While[
			LeafCount[y]>MaxComplexity,
			y=GenerateNormal[d]
			];
		y
		]

CrossoverProbability=0.9

(* Get list of all indices of internal points in expression *)
RemoveZero[x_]:=If[Position[x, 0]=={}, x, {}]
Points[x_]:=Union[Map[RemoveZero, Position[x, _]], {}]

GetInternal[{x___}]:=x

(* Perform crossover operation on two expressions *)
Cross1[x_, y_]:=
	Module[
		{spot1, spot2, point1, point2, temp1, temp2},
		If[
			Random[]<CrossoverProbability,
			point1=Points[x];
			spot1=Random[Integer, {1, Length[point1]}];
			point2=Points[y];
			spot2=Random[Integer, {1, Length[point2]}];
			temp1=x[[GetInternal[point1[[spot1]]]]];
			temp2=y[[GetInternal[point2[[spot2]]]]];
			{
				If[
					point1[[spot1]]=={},
					temp2,
					ReplacePart[x, temp2, point1[[spot1]]]
					],
				If[
					point2[[spot2]]=={},
					temp1,
					ReplacePart[y, temp1, point2[[spot2]]]
					]
				},
			{x, y}
			]
		]

MutationProbability=0.1

MaxSize=17

(* Perform mutation operation on an expression *)
Mutate[x_]:=
	Module[
		{spot1, point1, y, xold},
		xold=x;
		If[
			Random[]<MutationProbability,
			y=Generate[MaxInitialSize];
			point1=Points[x];
			spot1=Random[Integer, {1, Length[point1]}];
			If[
				point1[[spot1]]=={},
				y,
				ReplacePart[x, y, point1[[spot1]]]
				],
			If[
				((Depth[x]<MaxSize) && (LeafCount[x]<MaxComplexity)),
				x,
				xold
				]
			]
		]

(* RawFitness *)

StandardizedFitness[x_]:=RawFitness[x]

AdjustedFitness[x_]:=N[1/(1+StandardizedFitness[x])]

(* List of fitnesses of expressions in current generation *)
Fitnesses={}

(* Make cumulative fitnesses vector *)
CalcFitnessSum:=
	Module[{},
		FitSum=Table[Apply[Plus, Take[Fitnesses, i]], {i, 1, Length[Fitnesses]}];
		FitSum=Insert[FitSum, 0, 1];
		]

(* Bisection algorithm search for roulette wheel fitness choice *)
Search[x_] :=
	Module[
		{Mid, Start=1, Stop=Length[FitSum]},
		While[
			Start+1 != Stop,
			Mid = Floor[(Start+Stop)/2];
			If[
				FitSum[[Mid]] > x,
				Stop=Mid,
				Start=Mid
				]
			];
		Start
		]

(* Create new generation from previous one *)
NewGen[x_] :=
	Module[
		{maxwheel, newgen, lenx},
		newgen={};
		maxwheel=Apply[Plus, Fitnesses];
		lenx=Length[x];
		CalcFitnessSum;
		Do[
			Module[
				{spot, index, isum},
				spot=Random[]*maxwheel;
				index=Search[spot];
				newgen=Append[newgen, x[[index]]]
				],
			{i, 1, lenx}
			];
		newgen
		]

(* Perform crossover on all expressions in new generation *)
Crossover[x_] :=
	Module[
		{newx, oldx, n2, leno, origlen},
		oldx=x;
		newx={};
		leno=Length[oldx];
		origlen=leno;
		While[
			leno>0,
			If[
				leno==1,
				newx=Append[newx, First[oldx]];
				oldx=Rest[oldx],
				n2=Cross1[oldx[[1]], oldx[[2]]];
				If[
					((Depth[n2[[1]]]<=MaxSize) && (LeafCount[n2[[1]]]<=MaxComplexity)),
					newx=Append[newx, n2[[1]]],
					newx=Append[newx, oldx[[1]]]
					];
				If[
					((Depth[n2[[2]]]<=MaxSize) && (LeafCount[n2[[2]]]<=MaxComplexity)),
					newx=Append[newx, n2[[2]]],
					newx=Append[newx, oldx[[2]]]
					];
				oldx=Drop[oldx, 2];
				];
			leno=Length[oldx]
			];
		newx
		]

MaxGenerations=51
PopulationSize=250

(* Solution, SolutionFitness, SolutionSet *)

(* Update best-of-run individual *)
CheckSolution[gen_, x_]:=
	Module[
		{minf, maxf},
		Fitnesses=AdjustedFitness /@ x;
		minf=Position[Fitnesses, Min[Fitnesses]][[1,1]];
		maxf=Position[Fitnesses, Max[Fitnesses]][[1,1]];
		If[
			SolutionFitness<Fitnesses[[maxf]],
			Solution=x[[maxf]];
			SolutionFitness=Fitnesses[[maxf]]
			];
		SolutionSet=
			Append[SolutionSet,
				{gen, Fitnesses[[maxf]], x[[maxf]],
					Fitnesses[[minf]], x[[minf]]}
				];
		Print["G", gen, ": max ", Fitnesses[[maxf]],
						" min ", Fitnesses[[minf]]];
]

MinFitness=0.99

(* Generation, Population, TotTime *)

XTrans={}

(* Apply Genetic algorithm *)

ApplyGen :=
	Module[
		{onetime, poplog},
		Off[Get::noopen];
		Get["restart.log"];
		On[Get::noopen];
		newpop=Population;
		(* Print["G", Generation, ": calculating fitnesses ..."]; *)
		(* Print["G", Generation, ": done ... ", Timing[CheckSolution[Generation,newpop]][[1]]]; *)
		While[
			(SolutionFitness<MinFitness) && (Generation<MaxGenerations),
			onetime=
				Timing[
					Print["G", Generation, ": creating mating pool ..."];
					Print["G", Generation, ": done ... ", Timing[newpop=NewGen[newpop]][[1]]];
					Print["G", Generation, ": performing crossover ..."];
					Print["G", Generation, ": done ... ", Timing[newpop=Crossover[newpop]][[1]]];
					Print["G", Generation, ": performing mutation ..."];
					Print["G", Generation, ": done ... ", Timing[newpop=Map[Mutate, newpop]][[1]]];
					Generation++;
					Population=newpop;
					Print["G", Generation, ": calculating fitnesses ..."];
					Print["G", Generation, ": done ... ", Timing[CheckSolution[Generation, newpop]][[1]]];
					Print["G", Generation, ": best-of-run fitness so far = ", SolutionFitness];
					][[1]];
			Time[onetime, "G", Generation, ": total time for Generation change = "];
			TotTime+=onetime;
			Time[TotTime, "G", Generation, ": total time so far = "];
			Print["Saving state of system..."];
			Save["restart.log", PopulationSize];
			Save["restart.new", ContinueGen];
			RenameFile["restart.log", "restart.old"];
			RenameFile["restart.new", "restart.log"];
			DeleteFile["restart.old"];
			poplog=OpenAppend["pop.log"];
			WriteString[poplog, ","];
			Write[poplog, {Generation, Fitnesses}];
			Close[poplog];
			Print["Finished saving state of system..."];
			];
		{Solution /. XTrans, SolutionFitness}
		]

MaxInitialSize=6

(* Initialise Genetic algorithm *)
Initialize:=
	Block[
		{poplog},
		Population=Table[Generate[MaxInitialSize],
		{PopulationSize}];
		SolutionFitness=0;
		SolutionSet={};
		Generation=0;
		TotTime=0;
		Print["G", Generation, ": calculating fitnesses ..."];
		Print["G", Generation, ": done ... ", Timing[CheckSolution[Generation, Population]][[1]]];
		Print["G", Generation, ": best-of-run fitness so far = ", SolutionFitness];
		Off[DeleteFile::nffil];
		DeleteFile["pop.log"];
		DeleteFile["restart.log"];
		DeleteFile["restart.new"];
		DeleteFile["restart.old"];
		On[DeleteFile::nffil];
		poplog=OpenAppend["pop.log"];
		WriteString[poplog, "pop={"];
		Write[poplog, {Generation, Fitnesses}];
		Close[poplog];
		Save["restart.log", PopulationSize];
		Save["restart.log", ContinueGen];
		Information[Population];
		GInformation;
		]

(* Start run of algorithm *)
StartGen:=
	Timing[
		CheckAbort[
			ApplyGen,
			{Solution /. XTrans, SolutionFitness}
			]
		]

ContinueGen[gen_]:=
	Module[{},
		MaxGenerations=gen;
		MinFitness=2;
		Save["restart.log", MaxGenerations];
		Save["restart.log", MinFitness];
		StartGen
		]

GInformation:=
	Module[{},
		Print[""];
		Print["Population Size : ", PopulationSize];
		Print["Max no of Generations : ", MaxGenerations];
		Print["Max initial size : ", MaxInitialSize];
		Print["Max size : ", MaxSize];
		Print["Min solution fitness : ", MinFitness];
		Print["Mutation probability : ", MutationProbability];
		Print["Crossover probability : ", CrossoverProbability];
		Print["Terminal set : ", Terminals];
		Print["Function set : ", Functions];
		]
