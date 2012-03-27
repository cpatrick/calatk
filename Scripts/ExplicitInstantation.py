#!/usr/bin/env python

"""Create a .cxx file that performs explicit instantiation over float/double and
dimensions 1, 2, and 3.  Writes the file to the current directory."""

usage = "ExplicitInstantiation.py <class_name>"

import sys

if len(sys.argv) < 2 or sys.argv[1] == '-h' or sys.argv[1] == '--help':
    print(usage)
    sys.exit(1)

copyright_header = """/*
*
*  Copyright 2011 by the CALATK development team
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*
*/
"""

explicit_file = open(sys.argv[1] + '.cxx', 'w')
explicit_file.write(copyright_header)

content = """
#include "{0}.txx"

namespace CALATK
{

template class {0}< float, 1 >;
template class {0}< float, 2 >;
template class {0}< float, 3 >;
template class {0}< double, 1 >;
template class {0}< double, 2 >;
template class {0}< double, 3 >;

} // namespace CALATK
""".replace('{0}', sys.argv[1])
explicit_file.write(content)

explicit_file.close()
