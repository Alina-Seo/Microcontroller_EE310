#{
Serial Read program
  reads data from com5 serial port usually assigned to pic18f curiosity module, expecting 30 values

  Program: GNU Octave 8.4.0
  dependancies: instruemnt-control : install using pkg install command

  Output:
    writes to CSV file what it listens to on com5, in directory prompted to user

  Created by: Alina Matchette
  date: 5/5/2025

#}


mkdir randomOutput #have the user create output files for serial values and open files
edit randomOutput/outputs.csv
outCSV = fopen("outputs.csv",'w');

pkg load instrument-control #loads and innitializes serial values
s = serialport("COM5",9600);



for i = 1:30 #reads 30 values from com port 5, converts them to a number and stores that into cvs file along with ID of order recived
data = read(s,2);
conv = str2num(char(data));
fprintf(outCSV,"%d,%d\n",i,conv);

disp('recive')
end
s =[]; #closes port and file
fclose(outCSV);
