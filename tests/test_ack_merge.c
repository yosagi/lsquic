/* Copyright (c) 2017 - 2021 LiteSpeed Technologies Inc.  See LICENSE. */
/* Test ACK merge */

#include <assert.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#include "lsquic.h"
#include "lsquic_types.h"
#include "lsquic_int_types.h"
#include "lsquic_sizes.h"
#include "lsquic_parse.h"

struct test
{
    int                 lineno;
    int                 expect_failure;
    char                a[MAX_ACKI_STR_SZ];
    char                b[MAX_ACKI_STR_SZ];
    char                result[MAX_ACKI_STR_SZ];
};


static const struct test tests[] =
{

    {
        .lineno     = __LINE__,
        .expect_failure = 1,
        .a          = "",
        .b          = "[3-3]",
    },

    {
        .lineno     = __LINE__,
        .expect_failure = 1,
        .a          = "",
        .b          = "",
    },

    {
        .lineno     = __LINE__,
        .a          = "[3-3]",
        .b          = "[3-3]",
        .result     = "[3-3]",
    },

    {
        .lineno     = __LINE__,
        .a          = "[3-2]",
        .b          = "[1-1]",
        .result     = "[3-1]",
    },

    {
        .lineno     = __LINE__,
        .a          = "[15-15][3-2]",
        .b          = "[1-1]",
        .result     = "[15-15][3-1]",
    },

    {
        .lineno     = __LINE__,
        .a          = "[15-10][5-2]",
        .b          = "[9-6][1-1]",
        .result     = "[15-1]",
    },

    {
        .lineno     = __LINE__,
        .a          = "[15-10][5-2]",
        .b          = "[15-10][5-2]",
        .result     = "[15-10][5-2]",
    },

    {
        .lineno     = __LINE__,
        .a          = "[33803-33803][33800-33800][33788-33788][33775-33775][33759-33759][33744-33744][33732-33732][33717-33717][33706-33706][33691-33691][33679-33679][33664-33664][33649-33649][33638-33638][33622-33622][33613-33613][33585-33585][33574-33574][33562-33562][33546-33546][33531-33531][33516-33516][33504-33504][33490-33490][33483-33483][33481-32910][32906-32906][32894-32894][32880-32880][32869-32869][32854-32854][32844-32844][32817-32817][32806-32806][32791-32791][32780-32780][32766-32766][32752-32752][32741-32741][32726-32726][32716-32716][32702-32702][32697-32697][32682-32682][32672-32672][32657-32657][32651-32651][32636-32636][32626-32626][32611-32611][32600-32600][32589-32589][32574-32574][32564-32564][32551-32551][32536-32536][32525-32525][32511-32511][32500-32500][32485-32485][32475-32475][32460-32460][32449-32449][32438-32438][32423-32423][32412-32412][32399-32399][32385-32385][32375-32375][32360-32360][32349-32349][32334-32334][32326-32326][32312-32312][32302-32302][32287-32287][32277-32277][32262-32262][32252-32252][32239-32239][32224-32224][32214-32214][32200-32200][32190-32190][32175-32175][32161-32161][32151-32151][32136-32136][32126-32126][32111-32111][32103-32103][32090-32090][32080-32080][32069-32033][32008-30698][30696-30467]",
        .b          = "[33480-32910][32906-32906][32894-32894][32880-32880][32869-32869][32854-32854][32844-32844][32817-32817][32806-32806][32791-32791][32780-32780][32766-32766][32752-32752][32741-32741][32726-32726][32716-32716][32702-32702][32697-32697][32682-32682][32672-32672][32657-32657][32651-32651][32636-32636][32626-32626][32611-32611][32600-32600][32589-32589][32574-32574][32564-32564][32551-32551][32536-32536][32525-32525][32511-32511][32500-32500][32485-32485][32475-32475][32460-32460][32449-32449][32438-32438][32423-32423][32412-32412][32399-32399][32385-32385][32375-32375][32360-32360][32349-32349][32334-32334][32326-32326][32312-32312][32302-32302][32287-32287][32277-32277][32262-32262][32252-32252][32239-32239][32224-32224][32214-32214][32200-32200][32190-32190][32175-32175][32161-32161][32151-32151][32136-32136][32126-32126][32111-32111][32103-32103][32090-32090][32080-32080][32069-32033][32008-30698][30696-30467]",
        .result     = "[33803-33803][33800-33800][33788-33788][33775-33775][33759-33759][33744-33744][33732-33732][33717-33717][33706-33706][33691-33691][33679-33679][33664-33664][33649-33649][33638-33638][33622-33622][33613-33613][33585-33585][33574-33574][33562-33562][33546-33546][33531-33531][33516-33516][33504-33504][33490-33490][33483-33483][33481-32910][32906-32906][32894-32894][32880-32880][32869-32869][32854-32854][32844-32844][32817-32817][32806-32806][32791-32791][32780-32780][32766-32766][32752-32752][32741-32741][32726-32726][32716-32716][32702-32702][32697-32697][32682-32682][32672-32672][32657-32657][32651-32651][32636-32636][32626-32626][32611-32611][32600-32600][32589-32589][32574-32574][32564-32564][32551-32551][32536-32536][32525-32525][32511-32511][32500-32500][32485-32485][32475-32475][32460-32460][32449-32449][32438-32438][32423-32423][32412-32412][32399-32399][32385-32385][32375-32375][32360-32360][32349-32349][32334-32334][32326-32326][32312-32312][32302-32302][32287-32287][32277-32277][32262-32262][32252-32252][32239-32239][32224-32224][32214-32214][32200-32200][32190-32190][32175-32175][32161-32161][32151-32151][32136-32136][32126-32126][32111-32111][32103-32103][32090-32090][32080-32080][32069-32033][32008-30698][30696-30467]",
    },

};


static regex_t re;


static void
init_ranges (struct ack_info *a, const char *s)
{
    regmatch_t m[3];

    while (0 == regexec(&re, s, sizeof(m) / sizeof(m[0]), m, 0))
    {
        a->ranges[a->n_ranges].high = atoi(s + m[1].rm_so);
        a->ranges[a->n_ranges].low  = atoi(s + m[2].rm_so);
        /* Self-check: */
        assert(a->ranges[a->n_ranges].high >= a->ranges[a->n_ranges].low);
        ++a->n_ranges;
        s += m[0].rm_eo;
    }
}


static void
run_test_ext (const struct test *test, const char *a_str, const char *b_str)
{
    struct ack_info a, b;
    int s;
    char ack_str[MAX_ACKI_STR_SZ];

    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));

    init_ranges(&a, a_str);
    init_ranges(&b, b_str);

    s = lsquic_merge_acks(&a, &b);
    if (test->expect_failure)
    {
        assert(s != 0);
        return;
    }

    assert(s == 0);
    lsquic_acki2str(&a, ack_str, sizeof(ack_str));
    assert(0 == strcmp(ack_str, test->result));
}


static void
run_test (const struct test *test)
{
    run_test_ext(test, test->a, test->b);
    /* If flipped result should be the same: */
    run_test_ext(test, test->b, test->a);
}


static void
test_out_of_ranges (int success)
{
    struct ack_info a, b;
    int i, s;

    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));

    for (i = 0; i < 129 - success; ++i)
    {
        a.ranges[i].high = a.ranges[i].low = 100000 - i * 10;
        b.ranges[i].high = b.ranges[i].low = 100000 - 5 - i * 10;
    }
    a.n_ranges = i;
    b.n_ranges = i;

    s = lsquic_merge_acks(&a, &b);
    if (success)
        assert(s == 0);
    else
        assert(s != 0);
}


int
main (void)
{
    const struct test *test;
    int s;

    s = regcomp(&re, "\\[([0-9][0-9]*)-([0-9][0-9]*)\\]", REG_EXTENDED);
    assert(s == 0);

    for (test = tests; test < tests + sizeof(tests) / sizeof(tests[0]); ++test)
        run_test(test);

    test_out_of_ranges(0);
    test_out_of_ranges(1);

    regfree(&re);

    return 0;
}
