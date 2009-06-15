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

// $Id: test5_8.C,v 1.1 2008/10/30 19:21:12 legendre Exp $
/*
 * #Name: test5_8
 * #Desc: Declaration
 * #Dep: 
 * #Arch: sparc_sun_solaris2_4_test,i386_unknown_linux2_0_test,x86_64_unknown_linux2_4_test,ia64_unknown_linux2_4_test
 * #Notes:
 */

#include "BPatch.h"
#include "BPatch_Vector.h"
#include "BPatch_thread.h"
#include "BPatch_snippet.h"

#include "test_lib.h"

#include "dyninst_comp.h"
class test5_8_Mutator : public DyninstMutator {
public:
  virtual test_results_t executeTest();
};
extern "C" DLLEXPORT TestMutator *test5_8_factory() {
  return new test5_8_Mutator();
}

//
// Start Test Case #8 - (declaration)
//   
test_results_t test5_8_Mutator::executeTest() {
  // Find the exit point to the procedure "func_cpp"
  BPatch_Vector<BPatch_function *> bpfv;
  char *fn = "decl_test::func_cpp";
  if (NULL == appImage->findFunction(fn, bpfv) || !bpfv.size()
      || NULL == bpfv[0]){
    logerror( "**Failed** test #8 (declaration)\n");
    logerror( "    Unable to find function %s\n", fn);
    return FAILED;
  }
  BPatch_function *f1 = bpfv[0];  
  BPatch_Vector<BPatch_point *> *point8_1 = f1->findPoint(BPatch_exit);
  if (!point8_1 || (point8_1->size() < 1)) {
    logerror( "Unable to find point decl_test::func_cpp - exit.\n");
    return FAILED;
  }

  bpfv.clear();
  char *fn2 = "main";
  if (NULL == appImage->findFunction(fn2, bpfv) || !bpfv.size()
      || NULL == bpfv[0]){
    logerror( "**Failed** test #8 (declaration)\n");
    logerror( "    Unable to find function %s\n", fn2);
    return FAILED;
  }
  BPatch_function *f2 = bpfv[0];  
  BPatch_Vector<BPatch_point *> *point8_2 = f2->findPoint(BPatch_allLocations);
  if (!point8_2 || (point8_2->size() < 1)) {
    logerror( "Unable to find point in main.\n");
    return FAILED;
  }

  bpfv.clear();
  char *fn3 = "decl_test::call_cpp";
  if (NULL == appImage->findFunction(fn3, bpfv) || !bpfv.size()
      || NULL == bpfv[0]){
    logerror( "**Failed** test #8 (declaration)\n");
    logerror( "    Unable to find function %s\n", fn3);
    return FAILED;
  }
  BPatch_function *call8_func = bpfv[0];  

  BPatch_variableExpr *this8 = appImage->findVariable("test5_8_test8");
  if (this8 == NULL) {
    logerror( "**Failed** test #8 (declaration)\n");
    logerror( "Unable to find variable \"test5_8_test8\"\n");
    return FAILED;
  }

  BPatch_Vector<BPatch_snippet *> call8_args;
  BPatch_constExpr expr8_0((void *)this8->getBaseAddr());
  call8_args.push_back(&expr8_0);
  BPatch_constExpr expr8_1(8);
  call8_args.push_back(&expr8_1);
  BPatch_funcCallExpr call8Expr(*call8_func, call8_args);

  // find the variables of different scopes
  // What *exactly* are we testing here?  Just finding variables of various
  // types with different point parameters to findVariable()?
  BPatch_variableExpr *expr8_2=appImage->findVariable("CPP_DEFLT_ARG");
  BPatch_variableExpr *expr8_3=appImage->findVariable(*(*point8_2)[0], "test5_8_test8");
  BPatch_variableExpr *expr8_4=appImage->findVariable(*(*point8_1)[0], "CPP_DEFLT_ARG");
  if (!expr8_2 || !expr8_3 || !expr8_4) {
    logerror( "**Failed** test #8 (delcaration)\n");
    logerror( "    Unable to locate one of variables\n");
    return FAILED;
  }

    BPatch_Vector<BPatch_variableExpr *> *fields = expr8_3->getComponents();
    if (!fields || fields->size() == 0 ) {
          logerror( "**Failed** test #8 (declaration)\n");
          logerror( "  struct lacked correct number of elements\n");
          return FAILED;
     }

    unsigned int index = 0;
    while ( index < fields->size() ) {
	char fieldName[100];
	strcpy(fieldName, (*fields)[index]->getName());
       if ( !strcmp("CPP_TEST_UTIL_VAR", (*fields)[index]->getName()) ) {
           dprintf("Inserted snippet2\n");
           checkCost(call8Expr);
	   BPatchSnippetHandle *handle;
           handle = appThread->insertSnippet(call8Expr, *point8_1);
           return PASSED;
       }
       index ++;
    }
    logerror( "**Failed** test #8 (declaration)\n");
    logerror( "    Can't find inherited class member variables\n");
    return FAILED;
}