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

// $Id: test1_1.C,v 1.1 2008/10/30 19:17:22 legendre Exp $
/*
 *
 * #Name: test1_1
 * #Desc: Zero Argument Function Call
 * #Arch: all
 * #Dep: 
 */

#include "BPatch.h"
#include "BPatch_Vector.h"
#include "BPatch_thread.h"
#include "BPatch_snippet.h"

#include "test_lib.h"

#include "dyninst_comp.h"

class test1_1_Mutator : public DyninstMutator {
public:
  virtual test_results_t executeTest(); // override
};

// Factory function.
extern "C" DLLEXPORT TestMutator* test1_1_factory()
{
  return new test1_1_Mutator();
}

//
// Start Test Case #1 - (zero arg function call)
//
test_results_t test1_1_Mutator::executeTest() {
  const char *funcName = "test1_1_func1_1";
  const char* testName = "zero arg function call";
  int testNo = 1;

  // Find the entry point to the procedure "func1_1"
  
  BPatch_Vector<BPatch_function *> found_funcs;
  if ((NULL == appImage->findFunction(funcName, found_funcs))
      || !found_funcs.size()) {
    logerror("    Unable to find function %s\n", funcName);
    return FAILED;
  }
  
  if (1 < found_funcs.size()) {
    logerror("%s[%d]:  WARNING  : found %d functions named %s.  Using the first.\n", 
	    __FILE__, __LINE__, found_funcs.size(), funcName);
  }

  BPatch_Vector<BPatch_point *> *point1_1 = found_funcs[0]->findPoint(BPatch_entry);


  if (!point1_1 || ((*point1_1).size() == 0)) {
    logerror("**Failed** test #%d (%s)\n", testNo,testName);
    logerror("    Unable to find entry point to \"%s.\"\n", funcName);
    return FAILED;
  }

  BPatch_Vector<BPatch_function *> bpfv;
  char *fn = "test1_1_call1_1";
  if (NULL == appImage->findFunction(fn, bpfv) || !bpfv.size()
      || NULL == bpfv[0]){
    logerror("**Failed** test #%d (%s)\n", testNo, testName);
    logerror("    Unable to find function %s\n", fn);
    return FAILED;
  }
  BPatch_function *call1_func = bpfv[0];
  
  BPatch_Vector<BPatch_snippet *> call1_args;
  BPatch_funcCallExpr call1Expr(*call1_func, call1_args);

  checkCost(call1Expr);
  appAddrSpace->insertSnippet(call1Expr, *point1_1);
  dprintf("Inserted snippet\n");

  return PASSED;
} // test1_1_Mutator::executeTest()