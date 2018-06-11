#!/bin/bash

# Adapted from: Toluwanimi Salako
# Author:		Vincent Ho
# Author:		Abdullah Younis
# CS 199: 		Spring 2016

java_ais="Agents_java"
cpp_ais="Agents_cpp"
python_ais="Agents_python"
world_folder="Worlds"
	
rm -f Tournament_Results.txt

echo "==================== Starting tournament ===================="

if [ $(uname) == Linux ]; then
	module load python/3.5.1
	echo "RUNNING PYTHON AIS..."
	/pkg/python/3.5.1/bin/python3.5 bin/RunTournament.py $python_ais $world_folder

	echo "RUNNING JAVA AIS..."
	/pkg/java/1.8.0_20/bin/java -jar bin/javaScorer.jar $java_ais $world_folder

	echo "RUNNING C++ AIS..."
	for cppAgent in ./$cpp_ais/*/*
	do
		if [[ $cppAgent != *.exe ]]; then
			student_Name=$(echo $cppAgent | cut -d'/' -f3)
			for filename in $world_folder/*.txt; do $cppAgent $filename $cpp_ais/${student_Name}.txt; done
		fi
	done
	/pkg/java/1.8.0_20/bin/java -jar bin/cppScorer.jar ./$cpp_ais

	/pkg/python/3.5.1/bin/python3.5 bin/sort.py
else
	echo "RUNNING PYTHON AIS..."
	python bin/RunTournament.py $python_ais $world_folder

	echo "RUNNING JAVA AIS..."
	java -jar bin/javaScorer.jar $java_ais $world_folder

	echo "RUNNING C++ AIS..."
	for cppAgent in ./$cpp_ais/*/*
	do
		if [[ $cppAgent == *.exe ]]; then
			student_Name=$(echo $cppAgent | cut -d'/' -f3)
			for filename in $world_folder/*.txt; do $cppAgent $filename $cpp_ais/${student_Name}.txt; done
		fi
	done
	java -jar bin/cppScorer.jar ./$cpp_ais

	python bin/sort.py
fi

echo "==================== Ending tournament ===================="

rm -f $cpp_ais/*.txt
