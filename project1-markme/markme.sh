#!/bin/bash

PROJ="estimatecron"

if [ ! -r $PROJ.c ]; then
    echo "file $PROJ.c not found in the current directory"
    exit 1
fi

# -- NOTHING TO MODIFY BELOW HERE ------------------------------

TOPDIR=`pwd`
SAMPLEDIR="$TOPDIR/SAMPLE"
TESTDIR="$TOPDIR/TESTFILES"
#
SOLN="$SAMPLEDIR/$PROJ"
#
RUN4="$SAMPLEDIR/run4"
export RUN4_EXITSTATUS="/tmp/run4-exitstatus"
export RUN4_TIMEOUT="10sec"
#
CSTD="c99"
cc -std=$CSTD -Wall -Werror -o $RUN4 $RUN4.c	|| exit 1
cc -std=$CSTD -Wall -Werror -o $SOLN $SOLN.c	|| exit 1
#
export RUN="/tmp/RUN"

# --------------------------------------------------------------

if [ -t 1 ]; then
    RED="\033[1;31m" ; BLUE="\033[1;34m" ; OFF="\033[0m"
else
    RED="" ; BLUE="" ; OFF=""
fi

red () {
    echo -e "${RED}$1${OFF}"
}

blue () {
    echo -e "${BLUE}$1${OFF}"
}

display_output () {
    if [ -f $1 ]; then
	echo     "        $2"
	sed  's/^/            /' < $1
    fi
}

display_outputs () {
    display_output squash0 "your last output line (squashed):"
    display_output squash1 "sample's last output line (squashed):"
}

addmark () {
    echo "    $1 - $OUTCOME ($2 / $3)"
    MYMARK=`expr $MYMARK + $2`
    TOTALMARK=`expr $TOTALMARK + $3`
}

# --------------------------------------------------------------

preclean () {
    MARK="0"
    OUTCOME="Unsuccessful"
    rm -rf CRONTAB ESTIMATES SO SE output? squash? $RUN4_EXITSTATUS
}

# --------------------------------------------------------------

test0 () {
    preclean
    DESC="project compiled without warnings or errors"

    (cc -std=$CSTD -Wall -Werror \
                -Wno-logical-op-parentheses \
                -Wno-sometimes-uninitialized \
                -Wno-unused-variable \
                -Wno-tautological-overlap-compare \
                -Wno-parentheses \
                -o $PROJ $PROJ.c > SO) &> SE
#
# May be added on older macOS systems:
#                -Wno-unused-but-set-variable \
#
#
    if [ "$?" = "0" ]; then
	OUTCOME="Successful"
	MARK=3
    else
	red "compile failed"
	cat < SE
	MARK=0
	OUTCOME="Project did not compile."
	DESC="Unable to test project with automated tests."
    fi
    addmark ${FUNCNAME[0]} $MARK $1
}

# --------------------------------------------------------------

test1 () {
    preclean
    DESC="checks number of command-line arguments"

    $RUN4 $PROJ only-one-argument	&> output1
    ss=`cat $RUN4_EXITSTATUS`
    if [ "$ss" = "59" ]; then
	OUTCOME="TIMEOUT (after $RUN4_TIMEOUT)"
    else
	if [ "$ss" != "0" ]; then	# exits with non-zero?
	    MARK=`expr $MARK + 1`
	fi
	if [ -s output1 ]; then		# prints an error message?
	    MARK=`expr $MARK + 1`
	fi
	if [ "$MARK" = "$1" ]; then	# fully successful?
	    OUTCOME="Successful"
	fi
    fi
    addmark ${FUNCNAME[0]} $MARK $1
}

# --------------------------------------------------------------

test2 () {
    preclean
    DESC="detects a non-existent file, prints message, exits with failure"

    $RUN4 $PROJ 9 CRONTAB_DNE ESTIMATES_DNE	&> output1
    ss=`cat $RUN4_EXITSTATUS`
    if [ "$ss" = "59" ]; then
	OUTCOME="TIMEOUT (after $RUN4_TIMEOUT)"
    else
	if [ "$ss" != "0" ]; then
	    MARK=`expr $MARK + 1`
	fi
	if [ -s output1 ]; then
	    MARK=`expr $MARK + 1`
	fi
	if [ "$MARK" = "$1" ]; then
	    OUTCOME="Successful"
	fi
    fi
    addmark ${FUNCNAME[0]} $MARK $1
}

# --------------------------------------------------------------

# commontest  N  "description..."  MONTH  MARK
commontest () {
    preclean
    cp $TESTDIR/crontab$1	$RUN/CRONTAB
    cp $TESTDIR/estimates$1	$RUN/ESTIMATES
    DESC="$2"
    echo "    test$1"

    $RUN4 $PROJ $3 CRONTAB ESTIMATES	2>&1	> output0
    ss=`cat $RUN4_EXITSTATUS`
    if [ "$ss" = "59" ]; then
	OUTCOME="TIMEOUT (after $RUN4_TIMEOUT)"
    elif [ -s output0 ]; then
	$SOLN $3 CRONTAB ESTIMATES	2>&1	> output1

	tail -1 < output0 | tr -dc 'A-Za-z0-9\n' > squash0
	tail -1 < output1 | tr -dc 'A-Za-z0-9\n' > squash1
	display_outputs

	if cmp -s squash0 squash1 ; then
	    MARK="$4"				# full marks
	    OUTCOME="Successful"
	fi
    fi
    addmark test$1 $MARK $4
}

# --------------------------------------------------------------

prepare_automated_tests () {
    rm -rf $RUN ; mkdir $RUN
    cp -pR $PROJ.c $RUN
    echo >> $RUN/$PROJ.c	# append newline to source code
}

automated_tests () {
    cd $RUN

    MYMARK="0"
    TOTALMARK="0"

# 3 specific tests and their maximum mark
    test0  3
    test1  2
    test2  2

# 8 similar tests:   test#, description, first-argument, maximum mark
    commontest 3 "one command, no wildcards, numeric month; one estimate; month=1" 1 2
    commontest 4 "one command, no wildcards, alphabetic month; one estimate; month=1" 1 2
    commontest 5 "one daily command, wildcards; one estimate; month=8" 8 2
    commontest 6 "one daily command, wildcards; one estimate; month=may" may 2
    commontest 7 "one daily, one weekly command; many estimates; month=6" 6 3
    commontest 8 "three overlapping daily commands; three estimates; month=4" 4 3
    commontest 9 "a valid command that does not run in 2022; month=7" 7 2
    commontest 10 "three identical commands started at same time; month=9" 9 2
}

# --------------------------------------------------------------

find_student_info () {
    SS=`cat $TOPDIR/$PROJ.c | \
	sed -e 's/Student.:/Student:/' -e 's/Student .:/Student:/' | \
	grep -E -i "Student:" | cut -d: -f2 | \
	sed 's/[ 	]*//g' | tr -cd " 0-9,\n" | sort -u`

    SN1=`echo $SS | sed "s/\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\).*/\1/"`
    SN2=`echo $SS | sed "s/$SN1//" | tr -cd "0-9"`

#echo $SN1
    if [ "x$SN2" = "x" ] ; then
	SUBMITTERS="$SN1"
    else
#echo $SN2
	SUBMITTERS="$SN1 and $SN2"
    fi

    if [ "x$SUBMITTERS" = "x" ] ; then
	red "studno missing"
	kill -9 $$
    fi
}

# --------------------------------------------------------------

find_student_info $i
blue "Submission of - $SUBMITTERS" ;

prepare_automated_tests $i
automated_tests $i

blue "TOTAL OF AUTOMATED MARKING:  $MYMARK / $TOTALMARK"

rm -rf $RUN $RUN4 $RUN4_EXITSTATUS $SOLN
