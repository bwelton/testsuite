/*
 * Copyright (c) 1996-2004 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * This license is for research uses.  For such uses, there is no
 * charge. We define "research use" to mean you may freely use it
 * inside your organization for whatever purposes you see fit. But you
 * may not re-distribute Paradyn or parts of Paradyn, in any form
 * source or binary (including derivatives), electronic or otherwise,
 * to any other organization or entity without our permission.
 * 
 * (for other uses, please contact us at paradyn@cs.wisc.edu)
 * 
 * All warranties, including without limitation, any warranty of
 * merchantability or fitness for a particular purpose, are hereby
 * excluded.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * Even if advised of the possibility of such damages, under no
 * circumstances shall we (or any other person or entity with
 * proprietary rights in the software licensed hereunder) be liable
 * to you or any third party for direct, indirect, or consequential
 * damages of any character regardless of type of action, including,
 * without limitation, loss of profits, loss of use, loss of good
 * will, or computer failure or malfunction.  You agree to indemnify
 * us (and any other person or entity with proprietary rights in the
 * software licensed hereunder) for any and all liability it may
 * incur to third parties resulting from your use of Paradyn.
 */

// $Id: test1_17.C,v 1.1 2008/10/30 19:17:58 legendre Exp $
/*
 * #Name: test1_17
 * #Desc: Mutator Side - Return Values from func calls
 * #Dep: 
 * #Notes: Verifies that instrumentation inserted at exit point doesn't clobber return value
 */

#include "BPatch.h"
#include "BPatch_Vector.h"
#include "BPatch_thread.h"
#include "BPatch_snippet.h"

#include "test_lib.h"
#include "dyninst_comp.h"

class test1_17_Mutator : public DyninstMutator {
	virtual test_results_t executeTest();
};

extern "C" DLLEXPORT  TestMutator *test1_17_factory() 
{
	return new test1_17_Mutator();
}

// Start Test Case #17 - mutator side (return values from func calls)
// Verify that instrumentation inserted at a subroutine's exit point
// doesn't clobber its return value.
// Method: the mutatee's func17_1 (first and only) exit is instrumented to
// call call17_1 with parameter (constant) "1"; func17_2's (first and only)
// exit is similarly instrumented to call call17_2(1); a subsequent test in
// the mutatee compares the return values of func17_1 and func17_2.
// (No examination is made of the return values of call17_1 or call17_2.)
//

test_results_t test1_17_Mutator::executeTest() 
{
	// Find the entry point to the procedure "func17_1"

	const char *funcName = "test1_17_func1";
	BPatch_Vector<BPatch_function *> found_funcs;

	if ((NULL == appImage->findFunction(funcName, found_funcs))
			|| !found_funcs.size()) 
	{
		logerror("    Unable to find function %s\n", funcName);
		return FAILED;
	}

	if (1 < found_funcs.size()) 
	{
		logerror("%s[%d]:  WARNING  : found %d functions named %s.  Using the first.\n", 
				__FILE__, __LINE__, found_funcs.size(), funcName);
	}

	BPatch_Vector<BPatch_point *> *point17_1 = found_funcs[0]->findPoint(BPatch_exit);

	if (!point17_1 || (point17_1->size() < 1)) 
	{
		logerror("Unable to find point %s - exit.\n", funcName);
		return FAILED;
	}

	BPatch_Vector<BPatch_function *> bpfv;
	char *fn = "test1_17_call1";

	if (NULL == appImage->findFunction(fn, bpfv) || !bpfv.size()
			|| NULL == bpfv[0])
	{
		logerror("    Unable to find function %s\n", fn);
		return FAILED;
	}

	BPatch_function *call17_1_func = bpfv[0];

	BPatch_Vector<BPatch_snippet *> funcArgs;

	BPatch_variableExpr *var17_1 = appAddrSpace->malloc (*appImage->findType ("int"));
	BPatch_constExpr constExpr17_1 (0);
	BPatch_arithExpr arithExpr17_1 (BPatch_assign, *var17_1, BPatch_constExpr (1));
	appAddrSpace->insertSnippet (arithExpr17_1, *point17_1);

	int mutateeFortran = isMutateeFortran(appImage);

	if (mutateeFortran) 
	{
		constExpr17_1 = var17_1->getBaseAddr ();
	} 
	else 
	{
		constExpr17_1 = 1;
	}

	funcArgs.push_back (&constExpr17_1);

	BPatch_funcCallExpr call17_1Expr(*call17_1_func, funcArgs);
	checkCost(call17_1Expr);
	appAddrSpace->insertSnippet(call17_1Expr, *point17_1, BPatch_callAfter, BPatch_lastSnippet);

	// Find the exit point to the procedure "func17_2"
	const char *funcName2 = "test1_17_func2";
	BPatch_Vector<BPatch_function *> found_funcs2;

	if ((NULL == appImage->findFunction(funcName2, found_funcs2))
			|| !found_funcs2.size()) 
	{
		logerror("    Unable to find function %s\n", funcName2);
		return FAILED;
	}

	if (1 < found_funcs2.size()) 
	{
		logerror("%s[%d]:  WARNING  : found %d functions named %s.  Using the first.\n", 
				__FILE__, __LINE__, found_funcs2.size(), funcName2);
	}

	BPatch_Vector<BPatch_point *> *point17_2 = found_funcs2[0]->findPoint(BPatch_exit);

	if (!point17_2 || (point17_2->size() < 1)) 
	{
		logerror("Unable to find point %s - exit.\n", funcName2);
		return FAILED;
	}

	bpfv.clear();
	char *fn2 = "test1_17_call2";

	if (NULL == appImage->findFunction(fn2, bpfv) || !bpfv.size()
			|| NULL == bpfv[0])
	{
		logerror("    Unable to find function %s\n", fn2);
		return FAILED;
	}

	BPatch_function *call17_2_func = bpfv[0];

	BPatch_Vector<BPatch_snippet *> funcArgs2;

	BPatch_variableExpr *var17_2 = appAddrSpace->malloc (*appImage->findType ("int"));
	BPatch_constExpr constExpr17_2 (0);
	BPatch_arithExpr arith17_2 (BPatch_assign, *var17_2, BPatch_constExpr (1));
	appAddrSpace->insertSnippet (arith17_2, *point17_2);

	if (mutateeFortran) 
	{
		constExpr17_2 = var17_2->getBaseAddr ();
	} 
	else 
	{
		constExpr17_2 = 1;
	}

	funcArgs2.push_back (&constExpr17_2);

	BPatch_funcCallExpr call17_2Expr(*call17_2_func, funcArgs2);
	checkCost(call17_2Expr);

	// test interface to call into insertSnippet with only one parameter
	BPatch_point &aPoint = *(*point17_2)[0];
	appAddrSpace->insertSnippet(call17_2Expr, aPoint, BPatch_callAfter, BPatch_lastSnippet);

	return PASSED;
}