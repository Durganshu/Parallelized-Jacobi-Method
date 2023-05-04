for i in 1 2 3 4 5 6 7 8 9 10 
do
    # echo "Run $i" 
    echo "Run $i" >> graph1_log_batch.txt
    ./heat test.dat >> graph1_log_batch.txt
done
