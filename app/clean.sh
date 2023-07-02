echo "Processes with serial port opened:"
readarray -t serial_pids < <(fuser /dev/ttyACM0 | grep -Eo '[0-9]+')

for pid in ${serial_pids[@]}
do
	echo "--- $pid ---"
	echo "process command: $(ps -ef | grep "$pid")"
	echo "do you want to kill it (y/n)?"

	read answer

	if [[ "$answer" == 'y' ]]
	then
		kill -9 "$pid"
	fi
	
done

