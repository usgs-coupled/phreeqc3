
#ifndef TSV_INTERFACE_F_H
#define TSV_INTERFACE_F_H


int TSV_GetAllDataIndices(int *ix_fortran, int *iy_fortran, int *iz_fortran, int *it_fortran);
int TSV_GetCoordinateCounts(int *ix, int *iy, int *iz, int *it, int* count_data_lines);
int TSV_GetCoordinateLocations(double *x_loc, double *y_loc, double *z_loc, double *t_loc);
int TSV_GetParameter(char *name, double *all_data_column);
int TSV_Create(const char *file_name);
void TSV_interpolate(void);

#endif // TSV_INTERFACE_F_H