% reads cluster count and points from input files
function [NC points] = read_input_data(file_params, file_points)
	
	NC = load(file_params);
	format long; %specificare format; necesar pentru a considera toate zecimalele numerelor din fisierul de intrare
	load(file_points);

end

