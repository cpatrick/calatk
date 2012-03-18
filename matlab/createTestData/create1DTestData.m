clear all
close all

addpath ../nrrdUtilities

% create 2 1D curves which can be used as a registration test case
numberOfValues = 10000;
startValue = 0;
endValue = 1;

sourceCurveStart = 0.1;
sourceCurveEnd = 0.6;

X = [startValue:1/numberOfValues:endValue];
SourceCurve = zeros(1,numberOfValues+1);

for i=sourceCurveStart*numberOfValues:sourceCurveEnd*numberOfValues
    SourceCurve(i) = sin( pi*(X(i)-sourceCurveStart)/(sourceCurveEnd-sourceCurveStart) );
end
%subplot(2,1,1),plot(X, SourceCurve);

targetCurveStart = 0.3;
targetCurveEnd = 0.7

TargetCurve = zeros(1,numberOfValues+1);
for i=targetCurveStart*numberOfValues:targetCurveEnd*numberOfValues
    TargetCurve(i) = sin( pi*(X(i)-targetCurveStart)/(targetCurveEnd-targetCurveStart) );
end
%subplot(2,1,2),plot(X, TargetCurve);

% create a header
header = [];
header.sizes = [numberOfValues 1 1];
header.dimension = 3;
header.space = 'RAS';
header.kinds = {'space','space','space'};
header.spacedirections = [1 0 0 0 1 0 0 0 1]*1/numberOfValues;
header.spaceorigin = [0 0 0];

% now write this out
writeNrrdFileWithHeader( '1DCurveSource.nhdr', header, SourceCurve );
writeNrrdFileWithHeader( '1DCurveTarget.nhdr', header, TargetCurve );
