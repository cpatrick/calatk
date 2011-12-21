function k = getFFromIndex( iI, M, dx )

% gives the frequency

LM = M*dx;

if ( mod( M, 2 ) == 0 )
  % even
  MOff = 0;
else
  MOff = 1;
end

if ( iI>=(M+MOff)/2 )
  k = ( iI-M )/LM;
else
  k = iI/LM;
end
