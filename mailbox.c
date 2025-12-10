#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

static MAILBOX mailboxes[USERNUM];

static int read_line_local(char *buf, size_t sz)
{
    if (!fgets(buf, (int)sz, stdin)) return 0;
    size_t L = strlen(buf);
    if (L > 0 && buf[L-1] == '\n') buf[L-1] = '\0';
    return 1;
}

void init_mailboxes(void)
{
    for (int i = 0; i < USERNUM; ++i) {
        mailboxes[i].count = 0;
        for (int j = 0; j < MAX_VOICEMAILS; ++j) {
            mailboxes[i].from[j] = 0;
            mailboxes[i].messages[j][0] = '\0';
            mailboxes[i].read_flags[j] = 0;
        }
    }
}

int mailbox_add(int callee, int caller, const char *msg)
{
    if (callee < 1 || callee >= USERNUM) return -1;
    MAILBOX *mb = &mailboxes[callee];
    if (mb->count >= MAX_VOICEMAILS) return -1;

    int idx = mb->count;
    mb->from[idx] = caller;

    strncpy(mb->messages[idx], msg, MAX_VOICEMAIL_LEN - 1);
    mb->messages[idx][MAX_VOICEMAIL_LEN - 1] = '\0';

    mb->read_flags[idx] = 0;
    mb->count++;
    return idx;
}

void mailbox_list(int ln)
{
    if (ln < 1 || ln >= USERNUM) return;
    MAILBOX *mb = &mailboxes[ln];

    printf("\n=== 留守電一覧（端末 %d）===\n", ln);
    if (mb->count == 0) {
        printf("（メッセージなし）\n");
        return;
    }

    for (int i = 0; i < mb->count; i++) {
        printf("%2d. from [%d] %s\n",
            i + 1,
            mb->from[i],
            mb->read_flags[i] ? "(既読)" : "(未読)");
    }
}

void mailbox_play(int ln, int idx)
{
    if (ln < 1 || ln >= USERNUM) return;

    MAILBOX *mb = &mailboxes[ln];
    if (idx < 1 || idx > mb->count) {
        printf("番号が不正です\n");
        return;
    }

    int pos = idx - 1;

    printf("\n--- 再生（%d番） ---\n", idx);
    printf("送信者: %d\n", mb->from[pos]);
    printf("内容: %s\n\n", mb->messages[pos]);

    mb->read_flags[pos] = 1;
}

void mailbox_delete(int ln, int idx)
{
    if (ln < 1 || ln >= USERNUM) return;

    MAILBOX *mb = &mailboxes[ln];
    if (idx < 1 || idx > mb->count) {
        printf("番号が不正です\n");
        return;
    }

    int pos = idx - 1;

    // shift
    for (int i = pos; i < mb->count - 1; i++) {
        mb->from[i] = mb->from[i + 1];
        strcpy(mb->messages[i], mb->messages[i + 1]);
        mb->read_flags[i] = mb->read_flags[i + 1];
    }

    mb->count--;
    printf("メッセージ %d を削除しました\n", idx);
}

void offer_voicemail(int caller, int callee)
{
    char line[MAX_VOICEMAIL_LEN];

    printf("メッセージを残しますか？ Yes=1 No=0 >");
    fflush(stdout);
    if (!read_line_local(line, sizeof(line))) return;

    if (atoi(line) != 1) {
        printf("メッセージは残しませんでした\n");
        return;
    }

    printf("メッセージを入力してください（1行）>\n");
    fflush(stdout);

    if (!read_line_local(line, sizeof(line))) {
        printf("入力エラー\n");
        return;
    }

    if (mailbox_add(callee, caller, line) >= 0) {
        printf("メッセージを保存しました\n");
    } else {
        printf("留守電ボックスが満杯のため保存できませんでした\n");
    }
}
