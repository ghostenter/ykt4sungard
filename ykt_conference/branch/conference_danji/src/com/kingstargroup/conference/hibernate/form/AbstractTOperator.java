/*
 * WARNING: DO NOT EDIT THIS FILE. This is a generated file that is synchronized
 * by MyEclipse Hibernate tool integration.
 *
 * Created Fri Mar 31 15:14:55 CST 2006 by MyEclipse Hibernate Tool.
 */
package com.kingstargroup.conference.hibernate.form;

import java.io.Serializable;

/**
 * A class that represents a row in the T_OPERATOR table. 
 * You can customize the behavior of this class by editing the class, {@link TOperator()}.
 * WARNING: DO NOT EDIT THIS FILE. This is a generated file that is synchronized
 * by MyEclipse Hibernate tool integration.
 */
public abstract class AbstractTOperator 
    implements Serializable
{
    /** The cached hash code value for this instance.  Settting to 0 triggers re-calculation. */
    private int hashValue = 0;

    /** The composite primary key value. */
    private java.lang.String operCode;

    /** The value of the simple operName property. */
    private java.lang.String operName;

    /** The value of the simple operPwd property. */
    private java.lang.String operPwd;

    /** The value of the simple deptId property. */
    private java.lang.String deptId;

    /** The value of the simple loginTime property. */
    private java.lang.String loginTime;

    /** The value of the simple loginIp property. */
    private java.lang.String loginIp;

    /** The value of the simple status property. */
    private java.lang.String status;

    /**
     * Simple constructor of AbstractTOperator instances.
     */
    public AbstractTOperator()
    {
    }

    /**
     * Constructor of AbstractTOperator instances given a simple primary key.
     * @param operCode
     */
    public AbstractTOperator(java.lang.String operCode)
    {
        this.setOperCode(operCode);
    }

    /**
     * Return the simple primary key value that identifies this object.
     * @return java.lang.String
     */
    public java.lang.String getOperCode()
    {
        return operCode;
    }

    /**
     * Set the simple primary key value that identifies this object.
     * @param operCode
     */
    public void setOperCode(java.lang.String operCode)
    {
        this.hashValue = 0;
        this.operCode = operCode;
    }

    /**
     * Return the value of the OPER_NAME column.
     * @return java.lang.String
     */
    public java.lang.String getOperName()
    {
        return this.operName;
    }

    /**
     * Set the value of the OPER_NAME column.
     * @param operName
     */
    public void setOperName(java.lang.String operName)
    {
        this.operName = operName;
    }

    /**
     * Return the value of the OPER_PWD column.
     * @return java.lang.String
     */
    public java.lang.String getOperPwd()
    {
        return this.operPwd;
    }

    /**
     * Set the value of the OPER_PWD column.
     * @param operPwd
     */
    public void setOperPwd(java.lang.String operPwd)
    {
        this.operPwd = operPwd;
    }

    /**
     * Return the value of the DEPT_ID column.
     * @return java.lang.String
     */
    public java.lang.String getDeptId()
    {
        return this.deptId;
    }

    /**
     * Set the value of the DEPT_ID column.
     * @param deptId
     */
    public void setDeptId(java.lang.String deptId)
    {
        this.deptId = deptId;
    }

    /**
     * Return the value of the LOGIN_TIME column.
     * @return java.lang.String
     */
    public java.lang.String getLoginTime()
    {
        return this.loginTime;
    }

    /**
     * Set the value of the LOGIN_TIME column.
     * @param loginTime
     */
    public void setLoginTime(java.lang.String loginTime)
    {
        this.loginTime = loginTime;
    }

    /**
     * Return the value of the LOGIN_IP column.
     * @return java.lang.String
     */
    public java.lang.String getLoginIp()
    {
        return this.loginIp;
    }

    /**
     * Set the value of the LOGIN_IP column.
     * @param loginIp
     */
    public void setLoginIp(java.lang.String loginIp)
    {
        this.loginIp = loginIp;
    }

    /**
     * Return the value of the STATUS column.
     * @return java.lang.String
     */
    public java.lang.String getStatus()
    {
        return this.status;
    }

    /**
     * Set the value of the STATUS column.
     * @param status
     */
    public void setStatus(java.lang.String status)
    {
        this.status = status;
    }

    /**
     * Implementation of the equals comparison on the basis of equality of the primary key values.
     * @param rhs
     * @return boolean
     */
    public boolean equals(Object rhs)
    {
        if (rhs == null)
            return false;
        if (! (rhs instanceof TOperator))
            return false;
        TOperator that = (TOperator) rhs;
        if (this.getOperCode() == null || that.getOperCode() == null)
            return false;
        return (this.getOperCode().equals(that.getOperCode()));
    }

    /**
     * Implementation of the hashCode method conforming to the Bloch pattern with
     * the exception of array properties (these are very unlikely primary key types).
     * @return int
     */
    public int hashCode()
    {
        if (this.hashValue == 0)
        {
            int result = 17;
            int operCodeValue = this.getOperCode() == null ? 0 : this.getOperCode().hashCode();
            result = result * 37 + operCodeValue;
            this.hashValue = result;
        }
        return this.hashValue;
    }
}