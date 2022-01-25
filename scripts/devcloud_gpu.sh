#qsub -I -l nodes=1:iris_xe_max:ppn=2 -d .
qsub -I -l nodes=1:gpu:ppn=2 -d .
