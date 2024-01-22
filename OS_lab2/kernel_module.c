#include <linux/init.h> //для инициализации и завершения модуля
#include <linux/module.h> //подключение API :) модулей ядра
#include <linux/kernel.h> //pr_info
#include <linux/sysfs.h> // подключение API фаайловой системы sysfs. kobject и smsht
#include <linux/mm.h> //sysinfo, si_meminfo

#define MAX_BUF_LEN 256

static struct sysinfo info;

static ssize_t vm_stat_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    si_meminfo(&info);
    return snprintf(buf, PAGE_SIZE,
                    "Total RAM: %lu KB\n"
                    "Free RAM : %lu KB\n"
                    "Shared RAM: %lu KB\n"
                    "Buffered RAM: %lu KB\n"
                    "Total Swap: %lu KB\n"
                    "Free Swap: %lu KB\n"
                    "Number of processes: %u\n",
                    (unsigned long)info.totalram * (unsigned long)info.mem_unit / 1024,
                    (unsigned long)info.freeram * (unsigned long)info.mem_unit / 1024,
                    (unsigned long)info.sharedram * (unsigned long)info.mem_unit / 1024,
                    (unsigned long)info.bufferram * (unsigned long)info.mem_unit / 1024,
                    (unsigned long)info.totalswap * (unsigned long)info.mem_unit / 1024,
                    (unsigned long)info.freeswap * (unsigned long)info.mem_unit / 1024,
                    info.procs);
}

static struct kobj_attribute vm_stat_attribute = __ATTR_RO(vm_stat);

static struct attribute *attrs[] = {
        &vm_stat_attribute.attr,
        NULL,
};

static struct attribute_group attr_group = {
        .attrs = attrs,
};

static struct kobject *my_kobj;

static int __init my_module_init(void) {
    int result;

    my_kobj = kobject_create_and_add("my_module", kernel_kobj);
    if (!my_kobj)
        return -ENOMEM;

    result = sysfs_create_group(my_kobj, &attr_group);
    if (result)
        kobject_put(my_kobj);

    pr_info("My module loaded\n");
    return result;
}

static void __exit my_module_exit(void) {
    sysfs_remove_group(my_kobj, &attr_group);
    kobject_put(my_kobj);
    pr_info("My module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("pravalol");
MODULE_AUTHOR("Artemiy");
MODULE_DESCRIPTION("vm_stats_dlya_nischih");
