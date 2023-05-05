for i in 1 2 3 4 5 6 7 8 9 10 
do
    # echo "Run $i" 
    echo "Run $i" >> graph2_log_login.txt
    ./heat test.dat >> graph2_log_login.txt
done
