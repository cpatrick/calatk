function ret = myOwnTempName()

ret = char(fix(get_some_dev_random(20)'*26/256)+'a');

return

function x = get_some_dev_random(digits)
fid = fopen('/dev/random','r');
x = fread(fid, digits, 'uchar');
fclose(fid);

% from http://nibot-lab.livejournal.com/