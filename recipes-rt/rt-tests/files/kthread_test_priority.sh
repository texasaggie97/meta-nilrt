#!/bin/bash

source $(dirname "$0")/ptest-format.sh
ptest_test=$(basename "$0" ".sh")

# $1: name of task
# $2: policy it should have
# $3: priority it should have
function check_prio_for_task() {
	ptest_pass

	tokens=( `ps -e -o pid,comm | grep " $1"` )
	pid=${tokens[0]}

	policy_line=`chrt -p $pid | grep policy`
	regex=".*: SCHED_(.*)"
	[[ $policy_line =~ $regex ]]
	policy=${BASH_REMATCH[1]}

	if [ $policy != $2 ]; then
		echo "task $1 expected policy $2, but got policy $policy" >&2
		ptest_fail
	fi

	prio_line=`chrt -p $pid | grep priority`
	regex=".*: (.*)"
	[[ $prio_line =~ $regex ]]
	prio=${BASH_REMATCH[1]}

	if [ $prio != $3 ]; then
		echo "task $1 expected priority $3, but got priority $prio" >&2
		ptest_fail
	fi
}

# Check if there are any tasks running as SCHED_FIFO priority 1. On NILRT
# systems we expect this RT policy/priority level to be used only by the
# ksoftirqd/x tasks.
function check_tasks_fifo_1() {
	ptest_pass

	fifo_1_tasks=$(ps -e -x -o policy,rtprio,comm | awk '$1 == "FF" && $2 == 1 && $3 !~ /^ksoftirqd\/[0-9]+/ {print $3}')

	if [ ! -z "$fifo_1_tasks" ]; then
		echo "unexpected tasks found running at FIFO/1 priority:"
		echo "$fifo_1_tasks"
		ptest_fail
	fi
}

ptest_change_subtest 1 kthreadd
check_prio_for_task kthreadd FIFO 25
ptest_report
rc_first=$ptest_rc

ptest_change_subtest 2 ksoftirqd
check_prio_for_task ksoftirqd/0 FIFO 1
ptest_report

ptest_change_subtest 3 irq
check_prio_for_task irq/ FIFO 15
ptest_report

ptest_change_subtest 4 irq_work
check_prio_for_task irq_work/ FIFO 12
ptest_report

ptest_change_subtest 5 ktimers
check_prio_for_task ktimers/ FIFO 10
ptest_report

ptest_change_subtest 6 rcub
check_prio_for_task rcub/ FIFO 2
ptest_report

ptest_change_subtest 7 rcuc
check_prio_for_task rcuc/ FIFO 2
ptest_report

ptest_change_subtest 8 rcuog
check_prio_for_task rcuog/ FIFO 2
ptest_report

ptest_change_subtest 9 rcu_preempt
check_prio_for_task rcu_preempt FIFO 2
ptest_report

ptest_change_subtest 10 fifo_1
check_tasks_fifo_1
ptest_report

exit $rc_first
